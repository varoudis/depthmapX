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

#include "salalib/vgamodules/vgaisovist.h"
#include "salalib/isovist.h"

#include "genlib/stringutils.h"

bool VGAIsovist::run(Communicator *comm, MetaGraph &mgraph, const Options &, PointMap &map, bool simple_version) {
    map.m_hasIsovistAnalysis = false;
    // note, BSP tree plays with comm counting...
    comm->CommPostMessage(Communicator::NUM_STEPS, 2);
    comm->CommPostMessage(Communicator::CURRENT_STEP, 1);
    mgraph.makeBSPtree(comm);

    AttributeTable &attributes = map.getAttributeTable();

    comm->CommPostMessage(Communicator::CURRENT_STEP, 2);

    time_t atime = 0;
    if (comm) {
        qtimer(atime, 0);
        comm->CommPostMessage(Communicator::NUM_RECORDS, map.getFilledPointCount());
    }
    int count = 0;

    for (size_t i = 0; i < map.getCols(); i++) {
        for (size_t j = 0; j < map.getRows(); j++) {
            PixelRef curs = PixelRef(static_cast<short>(i), static_cast<short>(j));
            if (map.getPoint(curs).filled()) {
                count++;
                if (map.getPoint(curs).contextfilled() && !curs.iseven()) {
                    continue;
                }
                Isovist isovist;
                mgraph.makeIsovist(map.depixelate(curs), isovist);
                int row = attributes.getRowid(curs);

                isovist.setData(attributes, row, simple_version);
                Node &node = map.getPoint(curs).getNode();
                std::vector<PixelRef> *occ = node.m_occlusion_bins;
                size_t k;
                for (k = 0; k < 32; k++) {
                    occ[k].clear();
                    node.bin(k).setOccDistance(0.0f);
                }
                for (k = 0; k < isovist.getOcclusionPoints().size(); k++) {
                    const PointDist &pointdist = isovist.getOcclusionPoints().at(k);
                    int bin = whichbin(pointdist.m_point - map.depixelate(curs));
                    // only occlusion bins with a certain distance recorded (arbitrary scale note!)
                    if (pointdist.m_dist > 1.5) {
                        PixelRef pix = map.pixelate(pointdist.m_point);
                        if (pix != curs) {
                            occ[bin].push_back(pix);
                        }
                    }
                    node.bin(bin).setOccDistance(pointdist.m_dist);
                }
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
    map.m_hasIsovistAnalysis = true;

    return true;
}
