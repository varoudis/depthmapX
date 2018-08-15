// Copyright (C) 2017 Christian Sailer

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

#include "performancewriter.h"
#include <sstream>
#include <fstream>
#include <algorithm>


PerformanceWriter::PerformanceWriter(const std::string &filename) : m_filename(filename)
{
}


void PerformanceWriter::addData(const std::string &message, double timeInSeconds)
{
    std::stringstream ss;
    ss << "\"" << message << "\"," << timeInSeconds << "\n";
    m_data.push_back(ss.str());
}

void PerformanceWriter::write() const
{
    if (!m_filename.empty())
    {
        std::ofstream outfile(m_filename);
        outfile << "\"action\",\"duration\"\n";
        std::for_each(m_data.begin(), m_data.end(), [&outfile](const std::string& line)mutable ->void{(outfile) << line;});
        outfile << std::flush;
    }
}
