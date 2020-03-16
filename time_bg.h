#ifndef TIME_H
#define TIME_H

#include <iostream>
#include <ctime>

class Time
{
public:
    Time();
    ~Time();

    double GetDeltaTime();
    void Reset();
private:
    std::clock_t beginTime;
    std::clock_t lastTime;
};

#endif
