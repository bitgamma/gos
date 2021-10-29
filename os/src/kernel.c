#include "const.h"

void kernel_main() {
  for (int i = 0; i < 5000; i++) {
    ((uint8_t*)(VBE_MODE_INFO->framebuffer))[i] = i % 256;
  }

  while (1) {
    ;
  }
}