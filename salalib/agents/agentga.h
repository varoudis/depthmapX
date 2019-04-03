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

#include "agentprogram.h"

const int POPSIZE = 500;
// redo ASSAYs -- assaysize * assays (3 * 200 = 600 evaluations total)
// then take mean fitness: due to large variation in fitnesses with
// short assays such as this
const int ASSAYS = 3;
const int ASSAYSIZE = 25000;
const int GENERATIONS = 10000;
const int TIMESTEPS = 1600;

struct ProgramPopulation {
  public:
    AgentProgram m_population[POPSIZE];

  public:
    ProgramPopulation() {}
    AgentProgram *makeChild();
    void sort();
};
