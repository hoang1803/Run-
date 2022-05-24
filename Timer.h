#ifndef TIMER_H_


#define TIMER_H_


#include "BaseFunc.h"


class Timer
{
public:
    Timer();
    ~Timer();

    void start();
    void pause();
    void unpause();
    void stop();


    int getTicks();
    bool getIsStarted();
    bool getIsPaused();
private:

    int start_tick;
    int pause_tick;

    bool is_started;
    bool is_paused;
};



#endif // TIMER_H_
