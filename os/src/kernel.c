#include <timer.h>
#include <mem.h>
#include <2d.h>
#include <interrupt.h>
#include <pic.h>
#include <ps2.h>
#include <keyboard.h>


void test_main() {
  uint8_t* loading = (uint8_t*) (SYSTEM_ADDR + (512 * 5));
  uint8_t* loading2 = (uint8_t*) (loading + (512 * 938));

  kbd_event event;

  for (;;) {
    td_set_background(loading, 800, 600);

    while(!kbd_read(&event) || (event & 0xf000)) {
      sleep(10);
    }

    td_set_background(loading2, 800, 600);

    while(!kbd_read(&event) || (event & 0xf000)) {
      sleep(10);
    }
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
