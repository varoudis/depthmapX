#include "salalib/vgamodules/visuallocal.h"

#include "genlib/stringutils.h"

bool VGAVisualLocal::run(Communicator *comm, Options &options, PointMap &map, bool simple_version) {
    time_t atime = 0;
    if (comm) {
        qtimer( atime, 0 );
        comm->CommPostMessage( Communicator::NUM_RECORDS, map.getFilledPointCount() );
    }

    int cluster_col, control_col, controllability_col;
    if(!simple_version) {
        cluster_col = map.getAttributeTable().insertColumn("Visual Clustering Coefficient");
        control_col = map.getAttributeTable().insertColumn("Visual Control");
        controllability_col = map.getAttributeTable().insertColumn("Visual Controllability");
    }

    int count = 0;

    for (int i = 0; i < map.getCols(); i++) {
        for (int j = 0; j < map.getRows(); j++) {
            PixelRef curs = PixelRef( i, j );
            if ( map.getPoint( curs ).filled()) {
                if ((map.getPoint( curs ).contextfilled() && !curs.iseven()) ||
                        (options.gates_only)) {
                    count++;
                    continue;
                }
                int row = map.getAttributeTable().getRowid(curs);

                // This is much easier to do with a straight forward list:
                PixelRefVector neighbourhood;
                PixelRefVector totalneighbourhood;
                map.getPoint(curs).getNode().contents(neighbourhood);

                // only required to match previous non-stl output. Without this
                // the output differs by the last digit of the float
                std::sort(neighbourhood.begin(), neighbourhood.end());

                int cluster = 0;
                float control = 0.0f;


                for (size_t i = 0; i < neighbourhood.size(); i++) {
                    int intersect_size = 0, retro_size = 0;
                    Point& retpt = map.getPoint(neighbourhood[i]);
                    if (retpt.filled() && retpt.hasNode()) {
                        retpt.getNode().first();
                        while (!retpt.getNode().is_tail()) {
                            retro_size++;
                            if (std::find(neighbourhood.begin(), neighbourhood.end(), retpt.getNode().cursor()) != neighbourhood.end()) {
                                intersect_size++;
                            }
                            if (std::find(totalneighbourhood.begin(), totalneighbourhood.end(), retpt.getNode().cursor()) == totalneighbourhood.end()) {
                                totalneighbourhood.push_back(retpt.getNode().cursor()); // <- note add does nothing if member already exists
                            }
                            retpt.getNode().next();
                        }
                        control += 1.0f / float(retro_size);
                        cluster += intersect_size;
                    }
                }
#ifndef _COMPILE_dX_SIMPLE_VERSION
                if(!simple_version) {
                    if (neighbourhood.size() > 1) {
                        map.getAttributeTable().setValue(row, cluster_col, float(cluster / double(neighbourhood.size() * (neighbourhood.size() - 1.0))) );
                        map.getAttributeTable().setValue(row, control_col, float(control) );
                        map.getAttributeTable().setValue(row, controllability_col, float( double(neighbourhood.size()) / double(totalneighbourhood.size())) );
                    }
                    else {
                        map.getAttributeTable().setValue(row, cluster_col, -1 );
                        map.getAttributeTable().setValue(row, control_col, -1 );
                        map.getAttributeTable().setValue(row, controllability_col, -1 );
                    }
                }
 #endif
                count++;    // <- increment count
            }
            if (comm) {
                if (qtimer( atime, 500 )) {
                    if (comm->IsCancelled()) {
                        throw Communicator::CancelledException();
                    }
                    comm->CommPostMessage( Communicator::CURRENT_RECORD, count );
                }
            }
        }
    }

#ifndef _COMPILE_dX_SIMPLE_VERSION
    if(!simple_version)
        map.setDisplayedAttribute(cluster_col);
#endif

    return true;
}
