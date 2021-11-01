#include <timer.h>
#include <mem.h>
#include <vbe.h>
#include <interrupt.h>
#include <pic.h>
#include <ps2.h>
#include <keyboard.h>

void kernel_main() {
  idt_init();
  ps2_init();
  pic_init();
  systick_init();
  idt_enable();

  uint8_t* loading = (uint8_t*) (SYSTEM_ADDR + (512 * 4));
  uint8_t* fb = (uint8_t*)(VBE_MODE_INFO->framebuffer);

  kbd_event event;

  while(!kbd_read(&event)) {
    asm volatile ("nop");
  }

  for (uint32_t i = 0; i < VBE_MODE_INFO->height; i++) {
    for (uint32_t j = 0; j < VBE_MODE_INFO->width; j++) {
      *(fb++) = *(loading++);
    }
    fb += VBE_MODE_INFO->pitch - VBE_MODE_INFO->width;
  }

  for (;;) {
    asm volatile ("hlt");
  }
}
