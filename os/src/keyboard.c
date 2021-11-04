#include <keyboard.h>
#include <ps2.h>
#include <mem.h>
#include <port.h>
#include <queue.h>

#define KBD_PS2_EXTENDED0 0xE00000
#define KBD_PS2_EXTENDED1 0xE10000
#define KBD_PS2_RELEASED 0x00F000
#define KBD_PS2_F7 0x83

static uint32_t _partial_scancode = 0;
static queue_t _input_queue = (queue_t) { 0, 0, KBD_BUF_SIZE, (void *)KBD_BUF_ADDR};

kbd_event _kbd_ps2_remap(uint32_t scancode) {
  kbd_event evt = (scancode & KBD_PS2_RELEASED) == KBD_PS2_RELEASED ? KBD_RELEASED : 0;

  if ((scancode & KBD_PS2_EXTENDED1) == KBD_PS2_EXTENDED1) {
    evt |= (scancode & 0x7f) | 0x100;
  } else if ((scancode & KBD_PS2_EXTENDED0) == KBD_PS2_EXTENDED0) {
    evt |= (scancode & 0x7f) | 0x80;
  } else if ((scancode & 0xff) == KBD_PS2_F7) {
    evt |= 0x02;
  } else {
    evt |= (scancode & 0x7f);
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

  //TODO: add modifiers and repeat flag

  return true;
}
