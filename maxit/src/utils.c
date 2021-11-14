#include <utils.h>
#include <timer.h>
#include <keyboard.h>
#include <snd.h>

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

    system_run();
  }

  while(!timer_expired(&expiry) && !kbd_read(&evt)) {
    system_run();
  }
}