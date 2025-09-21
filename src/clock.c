#include "cat/clock.h"

#if defined(_WIN32)
    #include <windows.h>
#elif defined(__unix__)
    #include <time.h>
#endif

double catGetTime(void)
{
    double time = 0;
#if defined(_WIN32)
    LARGE_INTEGER now       = {0};
    LARGE_INTEGER frequency = {0};
    QueryPerformanceCounter(&now);
    QueryPerformanceFrequency(&frequency);
    time = (double)now.QuadPart / frequency.QuadPart;
#elif defined(__unix__)
    struct timespec now = {};
    clock_gettime(CLOCK_REALTIME, &now);
    time = (double)(now.tv_sec) + (now.tv_nsec) / 1000000000.f;
#endif
    return time;
}
