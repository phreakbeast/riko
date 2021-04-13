#ifndef _TEK_STOPWATCH_H
#define _TEK_STOPWATCH_H

#include "tek_types.h"

typedef struct stopwatch_t
{
    double started_time;
    double paused_time;
    bool is_started;
    bool is_paused;
}Stopwatch;

void stopwatch_start(Stopwatch* timer);

void stopwatch_stop(Stopwatch* timer);

void stopwatch_pause(Stopwatch* timer);

void stopwatch_unpause(Stopwatch* timer);

double stopwatch_get_seconds(Stopwatch* timer);

#endif
