// Timer header file
#include "types.h"

struct Timer {
  uint32  start_ticks;   // clock time
  uint32  paused_ticks;  // paused time
  bool    paused;        // is paused
  bool    started;       // is started
};

void Timer_Initialize(Timer* timer);
void Timer_Start(Timer* timer);
void Timer_Stop(Timer* timer);
void Timer_Pause(Timer* timer);
void Timer_Unpause(Timer* timer);
uint32 Timer_GetTicks(Timer* timer);