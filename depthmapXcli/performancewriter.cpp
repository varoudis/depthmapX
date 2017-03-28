#include "performancewriter.h"
#include <sstream>


PerformanceWriter::PerformanceWriter(const std::string &filename) : _filename(filename)
{
}


void PerformanceWriter::AddData(const string &message, double time)
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
        foreach(line, _data)
        {
            outfile << line;
        }
        outfile << std::flush();
    }
}
