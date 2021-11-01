#ifndef __KBD__
#define __KBD__

#include <stdint.h>
#include <stdbool.h>

typedef uint32_t kbd_event;

void kbd_ps2_rcv();
bool kbd_read(kbd_event *evt);

#endif