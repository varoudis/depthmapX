// sala - a component of the depthmapX - spatial network analysis platform
// Copyright (C) 2000-2010, University College London, Alasdair Turner
// Copyright (C) 2011-2012, Tasos Varoudis
// Copyright (C) 2019, Petros Koutsolampros

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

#include "agentset.h"

#include "salalib/pixelref.h"

AgentSet::AgentSet() {
    m_release_rate = 0.1;
    m_lifetime = 1000;
}

void AgentSet::init(int agent, int trail_num) {
    if (m_release_locations.size()) {
        int which = pafrand() % m_release_locations.size();
        agents[agent].onInit(m_release_locations[which], trail_num);
    } else {
        const PointMap &map = agents[agent].getPointMap();
        PixelRef pix;
        do {
            pix = map.pickPixel(prandom(m_release_locations_seed));
        } while (!map.getPoint(pix).filled());
        agents[agent].onInit(pix, trail_num);
    }
}

void AgentSet::move() {
    // go through backwards so remove does not affect later agents
    for (auto rev_iter = agents.rbegin(); rev_iter != agents.rend(); ++rev_iter) {
        rev_iter->onMove();
        if (rev_iter->getFrame() >= m_lifetime) {
            agents.erase(std::next(rev_iter).base());
        }
    }
}
