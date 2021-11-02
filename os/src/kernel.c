#include <timer.h>
#include <mem.h>
#include <2d.h>
#include <interrupt.h>
#include <pic.h>
#include <ps2.h>
#include <keyboard.h>

static void _wait_key() {
  kbd_event event;
  bool keyDepressed = false;

  while(!keyDepressed) {
    if (kbd_read(&event)) {
      keyDepressed = (event & 0xf000) == 0xf000;
    }
  }
}

void test_main() {
  td_image_t loading = {800, 600, (void *)(SYSTEM_ADDR + (512 * 5))};
  td_image_t loading2 = {800, 600, (void *)(loading.data + (512 * 938))};

  td_rect_t rect = {200, 300, 50, 60};

  for (;;) {
    td_set_background(&loading);

    _wait_key();

    td_set_background(&loading2);

    _wait_key();

    td_draw_solid_rect(&rect, 0xA5);

    _wait_key();

    td_clear_rect(&rect);

    _wait_key();
  }
}

void kernel_main() {
  idt_init();
  ps2_init();
  pic_init();
  systick_init();
  idt_enable();

  test_main();
}
