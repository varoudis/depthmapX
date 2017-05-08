#ifndef PERFORMANCEWRITER_H
#define PERFORMANCEWRITER_H

#include <vector>
#include <string>

class PerformanceWriter
{
private:
    std::vector<std::string> _data;
    std::string _filename;
public:
    PerformanceWriter(const std::string &filename);
    void AddData( const std::string &message, double time);
    void Write() const;
};

#endif // PERFORMANCEWRITER_H
