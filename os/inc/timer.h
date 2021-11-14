#ifndef __TIMER__
#define __TIMER__

#include <stdint.h>
#include <stdbool.h>
#include <mem.h>
#include <task.h>

typedef uint32_t timer_t;

inline void timer_start(timer_t *timer, uint32_t ms) {
  *timer = SYSTICK + ms;
}

inline bool timer_expired(timer_t *timer) {
  return SYSTICK >= *timer;
}

inline void yield() {
  task_run();
}

void systick_init();
void sleep(uint32_t ms);

#endif