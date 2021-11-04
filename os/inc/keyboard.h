#ifndef __KBD__
#define __KBD__

#include <stdint.h>
#include <stdbool.h>
#include <bitarray.h>

#define KBD_RELEASED 15
#define KBD_REPEAT 14
#define KBD_CTRL 13
#define KBD_SHIFT 12
#define KBD_ALT 11

#define KBD_IS_RELEASED(e) BIT_IS_SET(e, KBD_RELEASED)
#define KBD_IS_REPEAT(e) BIT_IS_SET(e, KBD_REPEAT)
#define KBD_ALT_MODIFIER(e) BIT_IS_SET(e, KBD_ALT)
#define KBD_SHIFT_SHIFT(e) BIT_IS_SET(e, KBD_SHIFT)
#define KBD_CTRL_MODIFIER(e) BIT_IS_SET(e, KBD_CTRL)

#define KBD_SCANCODE(k) (k & 0x1ff)

#define KBD_KEY_LALT 0x11
#define KBD_KEY_LSHIFT 0x12
#define KBD_KEY_LCTRL 0x15
#define KBD_KEY_RALT 0x91
#define KBD_KEY_RSHIFT 0x59
#define KBD_KEY_RCTRL 0x95

typedef uint16_t kbd_event;

void kbd_ps2_rcv();
bool kbd_read(kbd_event *evt);

#endif