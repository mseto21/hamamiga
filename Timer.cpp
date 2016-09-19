// Timer class

#include "Timer.hpp"
#include <SDL.h>

void Timer_Initialize(Timer* timer) {
  timer->start_ticks = 0;
  timer->paused_ticks = 0;
  timer->paused = false;
  timer->started = false;
}

void Timer_Start(Timer* timer) {
  timer->started = true;
  timer->paused = false;
  timer->start_ticks = SDL_GetTicks();
  timer->paused_ticks = 0;
}

void Timer_Stop(Timer* timer) {
  timer->started = false;
  timer->paused = false;
  timer->start_ticks = 0;
  timer->paused_ticks = 0;
}

void Timer_Pause(Timer* timer) {
  if (timer->started && timer->paused) {
    timer->paused = false;
    
    // Reset the starting ticks
    timer->start_ticks = SDL_GetTicks() - timer->paused_ticks;
    timer->paused_ticks = 0;
  }
}

void Timer_Unpause(Timer* timer) {
  if (timer->started && timer->paused) {
    timer->paused = false;
    
    // Reset the starting ticks
    timer->start_ticks = SDL_GetTicks() - timer->paused_ticks;
    timer->paused_ticks = 0;
  }
}

uint32 Timer_GetTicks(Timer* timer) {
  uint32 time = 0;
  if (timer->started) {
    if (timer->paused) {
      time = timer->paused_ticks;
    } else {
      time = SDL_GetTicks() - timer->start_ticks;
    }
  }
  return time;
}


