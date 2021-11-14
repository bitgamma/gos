#include <utils.h>
#include <timer.h>
#include <keyboard.h>
#include <snd.h>

void mxt_game_wait(uint32_t ms) {
  timer_t timer;
  timer_start(&timer, ms);

  while(!timer_expired(&timer)) {
    snd_run();
  }
}

void mxt_press_any_key(uint32_t timeout_ms) {
  timer_t expiry;
  kbd_event evt;

  timer_start(&expiry, timeout_ms);

  while(kbd_any_pressed()) {
    kbd_flush();

    if (timer_expired(&expiry)) {
      kbd_stuck();
      return;
    }

    snd_run();
  }

  while(!timer_expired(&expiry) && !kbd_read(&evt)) {
    snd_run();
  }
}