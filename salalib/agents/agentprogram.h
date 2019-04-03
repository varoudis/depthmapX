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

#include "genlib/p2dpoly.h"

#include <string>

struct AgentProgram {
    // comparative is comparative with current heading
    enum {
        SEL_LOS = 0x0001,
        SEL_LOS_OCC = 0x0002,
        SEL_TARGETTED = 0x1000,
        SEL_STANDARD = 0x1001,
        SEL_WEIGHTED = 0x1002,
        SEL_GIBSONIAN = 0x2000,
        SEL_LENGTH = 0x2001,
        SEL_OPTIC_FLOW = 0x2002,
        SEL_COMPARATIVE_LENGTH = 0x2003,
        SEL_COMPARATIVE_OPTIC_FLOW = 0x2004,
        SEL_GIBSONIAN2 = 0x4000,
        SEL_OPTIC_FLOW2 = 0x4001,
        SEL_OCCLUSION = 0x9000,
        SEL_OCC_ALL = 0x9001,
        SEL_OCC_BIN45 = 0x9002,
        SEL_OCC_BIN60 = 0x9003,
        SEL_OCC_STANDARD = 0x9004,
        SEL_OCC_WEIGHT_DIST = 0x9005,
        SEL_OCC_WEIGHT_ANG = 0x9006,
        SEL_OCC_WEIGHT_DIST_ANG = 0x9007,
        SEL_OCC_MEMORY = 0x9008
    };
    int m_sel_type;
    int m_steps;
    int m_vbin;
    // these three variables for evolved Gibsonian agents:
    int m_rule_order[4];
    float m_rule_threshold[4];
    float m_rule_probability[4];
    // these are for optic flow 2 agents
    int m_vahead;               // how wide your ahead vision is
    float m_ahead_threshold;    // will turn if neg flow greater than this threshold (set in range 1/100 to 1)
    float m_feeler_threshold;   // will turn if flow greater than this threshold (set in range 1 to 5)
    float m_feeler_probability; // turn with this much probability if a feeler triggers
    //
    // simple long range destinations:
    bool m_destination_directed;
    bool m_los_sqrd;
    //
    // if it is going to evolved, then have it remember its fitness:
    double m_fitness;
    //
    AgentProgram();
    //
    // for evolution
    void mutate();
    friend AgentProgram crossover(const AgentProgram &prog_a, const AgentProgram &prog_b);
    // to reload later:
    void save(const std::string &filename);
    bool open(const std::string &filename);
    std::vector<std::vector<Event2f>> m_trails;
};
