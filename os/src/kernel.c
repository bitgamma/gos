#include <timer.h>
#include <mem.h>
#include <interrupt.h>
#include <pic.h>
#include <ps2.h>
#include <cmos.h>
#include <rnd.h>

extern void main();

void _seed_rnd() {
  uint32_t s0 = 0, s1 = 0;
  uint32_t* bios_data = (uint32_t *)BIOS_DATA_ADDR;

  for (uint32_t i = 0; i < 128; i+=2) {
    s0 += *(bios_data++);
    s1 += cmos_read(i);
    s1 += *(bios_data++);
    s0 += cmos_read(i+1);
  }

  rnd_seed(s0, s1);
}

void kernel_main() {
  idt_init();
  _seed_rnd();
  ps2_init();
  pic_init();
  systick_init();
  idt_enable();

  main();

  for(;;) {
    asm volatile ("hlt");
  }
}
