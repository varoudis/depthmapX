#ifndef SIMPLETIMER_H
#define SIMPLETIMER_H
#include <chrono>

class SimpleTimer
{
public:
    SimpleTimer() : _startTime(std::chrono::high_resolution_clock::now())
    {
    }

    double getTimeInSeconds() const
    {
         auto t2 = std::chrono::high_resolution_clock::now();
        return static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(t2-_startTime).count()) / 1000.0;
    }

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> _startTime;
};

#endif // SIMPLETIMER_H
