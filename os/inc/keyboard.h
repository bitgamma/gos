#ifndef __KBD__
#define __KBD__

#include <stdint.h>
#include <stdbool.h>

#define KBD_RELEASED (1 << 15)
#define KBD_REPEAT (1 << 14)
#define KBD_SHIFT (1 << 13)
#define KBD_CTRL (1 << 12)
#define KBD_ALT (1 << 11)

typedef uint16_t kbd_event;

void kbd_ps2_rcv();
bool kbd_read(kbd_event *evt);

#endif