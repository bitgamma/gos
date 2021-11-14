#include <timer.h>
#include <pit.h>
#include <snd.h>

void systick_init() {
  SYSTICK = 0;
  pit_init();
}

void sleep(uint32_t ms) {
  timer_t timer;
  timer_start(&timer, ms);

  while(!timer_expired(&timer)) {
    yield();
  }
}

void yield() {
  snd_run();
}