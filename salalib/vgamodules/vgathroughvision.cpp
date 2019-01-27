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

#include "salalib/vgamodules/vgathroughvision.h"
#include "salalib/agents/agenthelpers.h"

#include "genlib/stringutils.h"

// This is a slow algorithm, but should give the correct answer
// for demonstrative purposes

bool VGAThroughVision::run(Communicator *comm, const Options &, PointMap &map, bool) {
    time_t atime = 0;
    if (comm) {
        qtimer(atime, 0);
        comm->CommPostMessage(Communicator::NUM_RECORDS, map.getFilledPointCount());
    }

    AttributeTable &attributes = map.getAttributeTable();

    // current version (not sure of differences!)
    for (size_t i = 0; i < map.getCols(); i++) {
        for (size_t j = 0; j < map.getRows(); j++) {
            PixelRef curs = PixelRef(static_cast<short>(i), static_cast<short>(j));
            map.getPoint(curs).m_misc = 0;
        }
    }

    int count = 0;
    for (size_t i = 0; i < map.getCols(); i++) {
        for (size_t j = 0; j < map.getRows(); j++) {
            pvecint seengates;
            PixelRef curs = PixelRef(static_cast<short>(i), static_cast<short>(j));
            Point &p = map.getPoint(curs);
            if (map.getPoint(curs).filled()) {
                p.getNode().first();
                while (!p.getNode().is_tail()) {
                    PixelRef x = p.getNode().cursor();
                    PixelRefVector pixels = map.quickPixelateLine(x, curs);
                    for (size_t k = 1; k < pixels.size() - 1; k++) {
                        map.getPoint(pixels[k]).m_misc += 1;
                        int index = attributes.getRowid(pixels[k]);

                        // TODO: Undocumented functionality. Shows how many times a gate is passed?
                        int gate = (index != -1) ? static_cast<int>(attributes.getValue(index, g_col_gate)) : -1;
                        if (gate != -1 && seengates.searchindex(gate) == paftl::npos) {
                            attributes.incrValue(index, g_col_gate_counts);
                            seengates.add(gate, paftl::ADD_HERE);
                        }
                    }
                    p.getNode().next();
                }
                // only increment count for actual filled points
                count++;
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

    int col = attributes.insertColumn("Through vision");

    for (size_t i = 0; i < attributes.getRowCount(); i++) {
        PixelRef curs = attributes.getRowKey(i);
        attributes.setValue(i, col, static_cast<float>(map.getPoint(curs).m_misc));
        map.getPoint(curs).m_misc = 0;
    }

    map.setDisplayedAttribute(-2);
    map.setDisplayedAttribute(col);

    return true;
}
