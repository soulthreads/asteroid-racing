#include "timer.h"


float Timer::getInitialTime() const
{
    return initialTime;
}

void Timer::setInitialTime(float value)
{
    initialTime = value;
    timeLeft = value;
    expired = false;
}

float Timer::getTimeLeft() const
{
    return timeLeft;
}

void Timer::tick ()
{
    if (timeLeft > 0) {
        timeLeft -= engine.delta / 1000.0;
    } else {
        timeLeft = 0;
        expired = true;
    }
}

void Timer::addTime (float t)
{
    timeLeft += t;
}

bool Timer::isExpired()
{
    return expired;
}

Timer::Timer()
{
}
