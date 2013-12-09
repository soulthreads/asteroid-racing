#ifndef TIMER_H
#define TIMER_H

#include "engine.h"

class Timer
{
    float initialTime;
    float timeLeft;
    bool expired;
public:
    Timer();
    float getInitialTime() const;
    void setInitialTime(float value);
    float getTimeLeft() const;
    void tick();
    bool isExpired ();
    void addTime(float t);
};

extern unique_ptr<Timer> timer;

#endif // TIMER_H
