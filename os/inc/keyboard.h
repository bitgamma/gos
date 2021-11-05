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
#define KBD_SHIFT_MODIFIER(e) BIT_IS_SET(e, KBD_SHIFT)
#define KBD_CTRL_MODIFIER(e) BIT_IS_SET(e, KBD_CTRL)

#define KBD_SCANCODE(k) (k & 0x1ff)

#define KBD_KEY_LALT 0x11
#define KBD_KEY_LSHIFT 0x12
#define KBD_KEY_LCTRL 0x14

#define KBD_KEY_RALT 0x91
#define KBD_KEY_RSHIFT 0x59
#define KBD_KEY_RCTRL 0x94

#define KBD_KEY_LEFT 0xeb
#define KBD_KEY_DOWN 0xf2
#define KBD_KEY_RIGHT 0xf4
#define KBD_KEY_UP 0xf5

#define KBD_KEY_ENTER 0x5a
#define KBD_KEY_SPACE 0x29
#define KBD_KEY_ESC 0x76

#define KBD_KEY_NUM_0 0x70
#define KBD_KEY_NUM_1 0x69
#define KBD_KEY_NUM_2 0x72
#define KBD_KEY_NUM_3 0x7a
#define KBD_KEY_NUM_4 0x6b
#define KBD_KEY_NUM_5 0x73
#define KBD_KEY_NUM_6 0x74
#define KBD_KEY_NUM_7 0x6c
#define KBD_KEY_NUM_8 0x75
#define KBD_KEY_NUM_9 0x7d

typedef uint16_t kbd_event;

void kbd_ps2_rcv();
bool kbd_read(kbd_event *evt);
void kbd_flush();

#endif