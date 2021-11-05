#include <keyboard.h>
#include <ps2.h>
#include <mem.h>
#include <port.h>
#include <queue.h>

#define KBD_PS2_EXTENDED0 0xe000
#define KBD_PS2_EXTENDED1 0xe100
#define KBD_PS2_RELEASED 0xf000

#define KBD_MAX_KEYS 512

static uint32_t _partial_scancode = 0;
static queue_t _input_queue = (queue_t) { 0, 0, KBD_BUF_SIZE, (void *)KBD_BUF_ADDR};
static bitarray_t _kbd_state[BA_SIZE(KBD_MAX_KEYS)];

kbd_event _kbd_ps2_remap(uint32_t scancode) {
  kbd_event evt = 0;

  if ((scancode & KBD_PS2_RELEASED) == KBD_PS2_RELEASED) {
    evt = (1 << KBD_RELEASED);
    scancode = (scancode >> 16) | (scancode & 0xff);
  }

  if ((scancode & KBD_PS2_EXTENDED1) == KBD_PS2_EXTENDED1) {
    evt |= (scancode & 0x7f) | 0x100;
  } else if ((scancode & KBD_PS2_EXTENDED0) == KBD_PS2_EXTENDED0) {
    evt |= (scancode & 0x7f) | 0x80;
  } else {
    // assumes that the only scancode with bit 7 set is 0x83 (F7) which conflicts with the undefiined 0xE003
    evt |= (scancode & 0xff);
  }

  return evt;
}

void kbd_ps2_rcv() {
  _partial_scancode |= inb(PS2_DATA_PORT);
  if ((_partial_scancode & 0xE0) == 0xE0) {
    _partial_scancode <<= 8;
  } else {
    queue_push_circular_overwrite_uint32(&_input_queue, _partial_scancode);
    _partial_scancode = 0;
  }
}

bool kbd_read(kbd_event *evt) {
  uint32_t scancode;
  if (!queue_read_circular_uint32(&_input_queue, &scancode)) {
    return false;
  }

  *evt = _kbd_ps2_remap(scancode);

  *evt |= (ba_get(_kbd_state, KBD_KEY_LALT) | ba_get(_kbd_state, KBD_KEY_RALT)) << KBD_ALT;
  *evt |= (ba_get(_kbd_state, KBD_KEY_LSHIFT) | ba_get(_kbd_state, KBD_KEY_RSHIFT)) << KBD_SHIFT;
  *evt |= (ba_get(_kbd_state, KBD_KEY_LCTRL) | ba_get(_kbd_state, KBD_KEY_RCTRL)) << KBD_CTRL;

  if (KBD_IS_RELEASED(*evt)) {
    ba_clear(_kbd_state, KBD_SCANCODE(*evt));
  } else {
    *evt |= ba_get(_kbd_state, KBD_SCANCODE(*evt)) << KBD_REPEAT;
    ba_set(_kbd_state, KBD_SCANCODE(*evt));
  }

  return true;
}

void kbd_flush() {
  _input_queue.read = _input_queue.write;
}
