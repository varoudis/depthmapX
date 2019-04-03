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

#pragma once

#include "agentset.h"

class AgentEngine {
  public: // public for now for speed
    std::vector<AgentSet> agentSets;
    int m_gatelayer;
    int m_timesteps;

  public:
    bool m_record_trails;
    int m_trail_count = 50;

  public:
    AgentEngine();
    void run(Communicator *comm, PointMap *pointmap);
    void insertTrailsInMap(ShapeMap& trailsMap);
};
