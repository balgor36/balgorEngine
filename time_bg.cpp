#include "time_bg.h"

Time::Time()
{

}

Time::~Time()
{

}

void Time::Reset(){
    lastTime = beginTime;
    beginTime = clock();
}

double Time::GetDeltaTime()
{
    return double(beginTime - lastTime) * 0.0001f;
}
