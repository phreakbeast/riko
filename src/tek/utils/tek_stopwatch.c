#include "tek_stopwatch.h"

#include "../platform/tek_clock.h"

void stopwatch_start(Stopwatch* timer)
{
    timer->started_time = time_get_seconds();
    timer->paused_time = 0;
    timer->is_started = true;
    timer->is_paused = false;
}

void stopwatch_stop(Stopwatch* timer)
{
    timer->started_time = 0;
    timer->paused_time = 0;
    timer->is_started = false;
    timer->is_paused = false;
}

void stopwatch_pause(Stopwatch* timer)
{
    if (timer->is_started && !timer->is_paused) 
    {
        timer->is_paused = true;
        timer->paused_time = time_get_seconds();
        timer->started_time = 0;
    }
}

void stopwatch_unpause(Stopwatch* timer)
{
    if (timer->is_started && timer->is_paused) 
    {
        timer->is_paused = false;
        timer->paused_time = 0;
        timer->started_time = time_get_seconds();
    }
}

double stopwatch_get_seconds(Stopwatch* timer)
{
    double res = 0;
    if (timer->is_started) 
    {
        if (timer->is_paused) 
        {
            res = timer->paused_time;
        } 
        else 
        {
            res = time_get_seconds() - timer->started_time;
        }
    }
    return res;
}