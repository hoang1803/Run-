#include "Timer.h"


Timer::Timer()
{
    start_tick = pause_tick = 0;

    is_started = is_paused = false;
}

Timer::~Timer()
{
    ;
}

void Timer::start()
{
    is_started = true;
    is_paused = false;
    start_tick = SDL_GetTicks();
}
void Timer::pause()
{
    if(is_started && !is_paused)
    {
        is_paused = true;
        pause_tick = SDL_GetTicks() - start_tick;
    }
}
void Timer::unpause()
{
    if(is_paused)
    {
        is_paused = false;
        start_tick = SDL_GetTicks() - pause_tick;
        pause_tick = 0;
    }
}
void Timer::stop()
{
    is_started = is_paused = false;
}


int Timer::getTicks()
{
    if(is_started)
    {
        if(is_paused)
            return pause_tick;
        return SDL_GetTicks() - start_tick;
    }
    return -1;
}
