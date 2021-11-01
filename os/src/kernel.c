#include <timer.h>
#include <mem.h>
#include <vbe.h>
#include <interrupt.h>
#include <pic.h>
#include <ps2.h>
#include <keyboard.h>


void test_main() {
  uint8_t* loading = (uint8_t*) (SYSTEM_ADDR + (512 * 4));
  uint8_t* loading2 = (uint8_t*) (loading + (512 * 938));

  kbd_event event;

  uint8_t* to_draw = loading;

  for (;;) {
    uint8_t* fb = (uint8_t*)(VBE_MODE_INFO->framebuffer);

    for (uint32_t i = 0; i < VBE_MODE_INFO->height; i++) {
      for (uint32_t j = 0; j < VBE_MODE_INFO->width; j++) {
        *(fb++) = *(to_draw++);
      }

      fb += VBE_MODE_INFO->pitch - VBE_MODE_INFO->width;
    }

    while(!kbd_read(&event) || event & 0xf000) {
      sleep(10);
    }

    if ((to_draw < loading2) && (to_draw > loading)) {
      to_draw = loading2;
    } else {
      to_draw = loading;
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
