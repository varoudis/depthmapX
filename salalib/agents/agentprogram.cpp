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

#include "agentprogram.h"

#include "genlib/pafmath.h"
#include "genlib/stringutils.h"

#include <fstream>

AgentProgram::AgentProgram() {
    m_sel_type = SEL_LOS;
    m_steps = 3;
    m_vbin = 7;
    m_destination_directed = false;
    m_los_sqrd = false;
}

void AgentProgram::mutate() {
    // do mutate rule order occassionally:
    if (pafrand() % 20 == 0) {
        // rule order relies on putting rules into slots:
        for (int i = 0; i < 4; i++) {
            m_rule_order[i] = -1;
        }
        for (int j = 0; j < 4; j++) {
            int choice = pafrand() % (4 - j);
            for (int k = 0; k < choice + 1; k++) {
                if (m_rule_order[k] != -1) {
                    choice++;
                }
            }
            m_rule_order[choice] = j;
        }
    }
    // mutate the rule threshold / probabilities
    for (int i = 0; i < 4; i++) {
        if (pafrand() % 20 == 0) { // 5% mutation rate
            m_rule_threshold[i] = float(prandom() * 100.0);
        }
        if (pafrand() % 20 == 0) { // 5% mutation rate
            m_rule_probability[i] = float(prandom());
        }
    }
}

AgentProgram crossover(const AgentProgram &prog_a, const AgentProgram &prog_b) {
    AgentProgram child;

    // either one rule priority order or the other (don't try to mix!)
    if (pafrand() % 2) {
        for (int i = 0; i < 4; i++) {
            child.m_rule_order[i] = prog_a.m_rule_order[i];
        }
    } else {
        for (int i = 0; i < 4; i++) {
            child.m_rule_order[i] = prog_b.m_rule_order[i];
        }
    }
    // for each rule, either one rule threshold / probability or the other:
    for (int i = 0; i < 4; i++) {
        if (pafrand() % 2) {
            child.m_rule_threshold[i] = prog_a.m_rule_threshold[i];
        } else {
            child.m_rule_threshold[i] = prog_b.m_rule_threshold[i];
        }
        if (pafrand() % 2) {
            child.m_rule_probability[i] = prog_a.m_rule_probability[i];
        } else {
            child.m_rule_probability[i] = prog_b.m_rule_probability[i];
        }
    }

    return child;
}

// TODO: Expose this functionality to the UIs
void AgentProgram::save(const std::string &filename) {
    // standard ascii:
    std::ofstream file(filename.c_str());

    file << "Destination selection: ";
    switch (m_sel_type) {
    case SEL_STANDARD:
        file << "Standard" << std::endl;
        break;
    case SEL_LENGTH:
        file << "Gibsonian Length" << std::endl;
        break;
    case SEL_OPTIC_FLOW:
        file << "Gibsonian Optic Flow" << std::endl;
        break;
    case SEL_COMPARATIVE_LENGTH:
        file << "Gibsonian Comparative Length" << std::endl;
        break;
    case SEL_COMPARATIVE_OPTIC_FLOW:
        file << "Gibsonian Comparative Optic Flow" << std::endl;
        break;
    default:
        file << "Unknown" << std::endl;
    }

    file << "Steps: " << m_steps << std::endl;
    file << "Bins: " << ((m_vbin == -1) ? 32 : m_vbin * 2 + 1) << std::endl;
    file << "Rule order: " << m_rule_order[0] << " " << m_rule_order[1] << " " << m_rule_order[2] << " "
         << m_rule_order[3] << std::endl;

    for (int i = 0; i < 4; i++) {
        file << "Rule " << i << " (Bin -" << 1 + (i * 2) << "/+" << 1 + (i * 2) << ")" << std::endl;
        file << "Threshold: " << m_rule_threshold[i] << std::endl;
        file << "Turn Probability: " << m_rule_probability[i] << std::endl;
    }

    file << "Fitness: " << m_fitness << std::endl;
}

bool AgentProgram::open(const std::string &filename) {
    // standard ascii:
    std::ifstream file(filename.c_str());

    std::string line;
    file >> line;
    if (!line.empty()) {
        dXstring::toLower(line);
        if (line.substr(0, 22) != "destination selection:") {
            return false;
        } else {
            std::string method = line.substr(22);
            dXstring::ltrim(method);
            if (!method.empty()) {
                if (method == "standard") {
                    m_sel_type = SEL_STANDARD;
                } else if (method == "gibsonian length") {
                    m_sel_type = SEL_LENGTH;
                } else if (method == "gibsonian optic flow") {
                    m_sel_type = SEL_OPTIC_FLOW;
                } else if (method == "gibsonian comparative length") {
                    m_sel_type = SEL_COMPARATIVE_LENGTH;
                } else if (method == "gibsonian comparative optic flow") {
                    m_sel_type = SEL_COMPARATIVE_OPTIC_FLOW;
                }
                file >> line;
            } else {
                return false;
            }
        }
    } else {
        return false;
    }

    bool foundsteps = false;
    bool foundbins = false;

    if (!line.empty()) {
        dXstring::toLower(line);
        if (line.substr(0, 6) == "steps:") {
            std::string steps = line.substr(6);
            dXstring::ltrim(steps);
            m_steps = stoi(steps);
            file >> line;
            foundsteps = true;
        }
    } else {
        return false;
    }

    if (!line.empty()) {
        dXstring::toLower(line);
        if (line.substr(0, 5) == "bins:") {
            std::string bins = line.substr(6);
            dXstring::ltrim(bins);
            int binx = stoi(bins);
            if (binx == 32) {
                m_vbin = -1;
            } else {
                m_vbin = (atoi(bins.c_str()) - 1) / 2;
            }
            file >> line;
            foundbins = true;
        }
    }

    if (m_sel_type == SEL_STANDARD) {
        if (foundbins && foundsteps) {
            return true;
        } else {
            return false;
        }
    }

    if (!line.empty()) {
        dXstring::toLower(line);
        if (line.substr(0, 11) == "rule order:") {
            std::string ruleorder = line.substr(11);
            dXstring::ltrim(ruleorder);
            auto orders = dXstring::split(ruleorder, ' ');
            if (orders.size() != 4) {
                return false;
            }
            for (int i = 0; i < 4; i++) {
                m_rule_order[i] = stoi(orders[i]);
            }
            file >> line;
        } else {
            return false;
        }
    } else {
        return false;
    }
    for (int i = 0; i < 4; i++) {
        if (!line.empty()) {
            dXstring::toLower(line);
            if (line.substr(0, 4) == "rule") {
                file >> line;
            }
            dXstring::toLower(line);
            if (line.substr(0, 10) == "threshold:") {
                auto threshold = line.substr(10);
                dXstring::ltrim(threshold);
                m_rule_threshold[i] = stof(threshold);
                file >> line;
            } else {
                return false;
            }
            dXstring::toLower(line);
            if (line.substr(0, 17) == "turn probability:") {
                auto prob = line.substr(17);
                dXstring::ltrim(prob);
                m_rule_probability[i] = stof(prob);
                file >> line;
            } else {
                return false;
            }
        } else {
            return false;
        }
    }

    return true;
}
