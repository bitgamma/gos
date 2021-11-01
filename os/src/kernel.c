#include <mem.h>
#include <vbe.h>
#include <interrupt.h>
#include <pic.h>

void kernel_main() {
  pic_init();
  idt_init();
  idt_enable();

  uint8_t* loading = (uint8_t*) (SYSTEM_ADDR + (512 * 3));
  uint8_t* fb = (uint8_t*)(VBE_MODE_INFO->framebuffer);

  for (uint32_t i = 0; i < VBE_MODE_INFO->height; i++) {
    for (uint32_t j = 0; j < VBE_MODE_INFO->width; j++) {
      *(fb++) = *(loading++);
    }

    fb += VBE_MODE_INFO->pitch - VBE_MODE_INFO->width;
  }

  for (;;) {
    asm("hlt");
  }
}
