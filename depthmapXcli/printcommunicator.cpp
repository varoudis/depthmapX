// Copyright (C) 2020, Petros Koutsolampros

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

#include <iostream>
#include "depthmapXcli/printcommunicator.h"

void PrintCommunicator::CommPostMessage(int m, int x) const {
    switch (m) {
    case Communicator::NUM_STEPS:
        num_steps = x;
        break;
    case Communicator::CURRENT_STEP:
        step = x;
        break;
    case Communicator::NUM_RECORDS:
        num_records = x;
        break;
    case Communicator::CURRENT_RECORD:
        record = x;
        if (record > num_records) record = num_records;
        std::cout << "step: " << step << "/" << num_steps << " "
                  << "record: " << record << "/" << num_records << std::endl;
        break;
    default:
        break;
    }
}
