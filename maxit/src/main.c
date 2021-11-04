#include <keyboard.h>
#include <2d.h>
#include <res.h>
#include <dbg.h>

static void _wait_key() {
  kbd_event event;
  bool keyDepressed = false;

  while(!keyDepressed) {
    if (kbd_read(&event)) {
      dbg_log_uint16(event);
      keyDepressed = (event & KBD_RELEASED) == KBD_RELEASED;
    }
  }
}

void main() {
  td_rect_t rect = {250, 150, 200, 300};
  res_init();

  for (;;) {
    td_set_background(&res_mainmenu);
    _wait_key();

    td_set_background(&res_girl1);
    _wait_key();

    td_draw_solid_rect(&rect, 0xA5);
    _wait_key();

    td_clear_rect(&rect);
    _wait_key();

    td_draw_border_rect(&rect, 0x4, 3);
    _wait_key();

    td_clear_border_rect(&rect, 3);
    _wait_key();
  }
}