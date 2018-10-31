// sala - a component of the depthmapX - spatial network analysis platform
// Copyright (C) 2000-2010, University College London, Alasdair Turner
// Copyright (C) 2011-2012, Tasos Varoudis
// Copyright (C) 2017-2018, Petros Koutsolampros

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "salalib/vgamodules/angular.h"

#include "genlib/stringutils.h"

// This is a slow algorithm, but should give the correct answer
// for demonstrative purposes

bool VGAAngular::run(Communicator *comm, const Options &options, PointMap &map, bool) {
    time_t atime = 0;
    if (comm) {
        qtimer(atime, 0);
        comm->CommPostMessage(Communicator::NUM_RECORDS, map.getFilledPointCount());
    }

    std::string radius_text;
    if (options.radius != -1.0) {
        if (map.getRegion().width() > 100.0) {
            radius_text = std::string(" R") + dXstring::formatString(options.radius, "%.f");
        } else if (map.getRegion().width() < 1.0) {
            radius_text = std::string(" R") + dXstring::formatString(options.radius, "%.4f");
        } else {
            radius_text = std::string(" R") + dXstring::formatString(options.radius, "%.2f");
        }
    }

    AttributeTable &attributes = map.getAttributeTable();

    // n.b. these must be entered in alphabetical order to preserve col indexing:
    std::string mean_depth_col_text = std::string("Angular Mean Depth") + radius_text;
    int mean_depth_col = attributes.insertColumn(mean_depth_col_text.c_str());
    std::string total_detph_col_text = std::string("Angular Total Depth") + radius_text;
    int total_depth_col = attributes.insertColumn(total_detph_col_text.c_str());
    std::string count_col_text = std::string("Angular Node Count") + radius_text;
    int count_col = attributes.insertColumn(count_col_text.c_str());

    int count = 0;

    for (size_t i = 0; i < map.getCols(); i++) {
        for (size_t j = 0; j < map.getRows(); j++) {
            PixelRef curs = PixelRef(static_cast<short>(i), static_cast<short>(j));

            if (map.getPoint(curs).filled()) {

                if (options.gates_only) {
                    count++;
                    continue;
                }

                // TODO: Break out miscs/dist/cumangle into local variables and remove from Point class
                for (auto &point : map.getPoints()) {
                    point.m_misc = 0;
                    point.m_dist = 0.0f;
                    point.m_cumangle = -1.0f;
                }

                float total_angle = 0.0f;
                int total_nodes = 0;

                // note that m_misc is used in a different manner to analyseGraph / PointDepth
                // here it marks the node as used in calculation only

                std::set<AngularTriple> search_list;
                search_list.insert(AngularTriple(0.0f, curs, NoPixel));
                map.getPoint(curs).m_cumangle = 0.0f;
                while (search_list.size()) {
                    std::set<AngularTriple>::iterator it = search_list.begin();
                    AngularTriple here = *it;
                    search_list.erase(it);
                    if (options.radius != -1.0 && here.angle > options.radius) {
                        break;
                    }
                    Point &p = map.getPoint(here.pixel);
                    // nb, the filled check is necessary as diagonals seem to be stored with 'gaps' left in
                    if (p.filled() && p.m_misc != ~0) {
                        p.getNode().extractAngular(search_list, &map, here);
                        p.m_misc = ~0;
                        if (!p.getMergePixel().empty()) {
                            Point &p2 = map.getPoint(p.getMergePixel());
                            if (p2.m_misc != ~0) {
                                p2.m_cumangle = p.m_cumangle;
                                p2.getNode().extractAngular(search_list, &map,
                                                          AngularTriple(here.angle, p.getMergePixel(), NoPixel));
                                p2.m_misc = ~0;
                            }
                        }
                        total_angle += p.m_cumangle;
                        total_nodes += 1;
                    }
                }

                int row = attributes.getRowid(curs);
                if (total_nodes > 0) {
                    attributes.setValue(row, mean_depth_col, float(double(total_angle) / double(total_nodes)));
                }
                attributes.setValue(row, total_depth_col, total_angle);
                attributes.setValue(row, count_col, float(total_nodes));

                count++; // <- increment count
            }
            if (comm) {
                if (qtimer(atime, 500)) {
                    if (comm->IsCancelled()) {
                        throw Communicator::CancelledException();
                    }
                    comm->CommPostMessage(Communicator::CURRENT_RECORD, count);
                }
            }
        }
    }

    map.setDisplayedAttribute(-2);
    map.setDisplayedAttribute(mean_depth_col);

    return true;
}
