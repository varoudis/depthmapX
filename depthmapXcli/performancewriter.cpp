#include "performancewriter.h"
#include <sstream>
#include <fstream>
#include <algorithm>


PerformanceWriter::PerformanceWriter(const std::string &filename) : _filename(filename)
{
}


void PerformanceWriter::AddData(const std::string &message, double time)
{
    std::stringstream ss;
    ss << "\"" << message << ",\"" << time << "\n";
    _data.push_back(ss.str());
}

void PerformanceWriter::Write() const
{
    if (!_filename.empty())
    {
        std::ofstream outfile(_filename);
        outfile << "\"action\",\"duration\"\n";
        std::for_each(_data.begin(), _data.end(), [&outfile](auto line)mutable ->void{(outfile) << line;});
        outfile << std::flush;
    }
}
