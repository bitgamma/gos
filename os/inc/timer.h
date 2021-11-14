#ifndef __TIMER__
#define __TIMER__

#include <stdint.h>
#include <stdbool.h>
#include <mem.h>

typedef uint32_t timer_t;

inline void timer_start(timer_t *timer, uint32_t ms) {
  *timer = SYSTICK + ms;
}

inline bool timer_expired(timer_t *timer) {
  return SYSTICK >= *timer;
}

void systick_init();
void sleep(uint32_t ms);
void yield();

#endif