#include "../tek_clock.hpp"

#include <time.h>

namespace platform
{
    
    static struct timespec g_start_ticks;
    
    void time_init()
    {
        clock_gettime(CLOCK_REALTIME, &g_start_ticks);
    }
    
    double time_get_seconds()
    {
        struct timespec now;
        clock_gettime(CLOCK_REALTIME, &now);
        unsigned int res = (now.tv_sec - g_start_ticks.tv_sec) * 1000 + (now.tv_nsec - g_start_ticks.tv_nsec) / 1000000;
        return res / 1000.0;
    }
    
}