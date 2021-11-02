#include <keyboard.h>
#include <ps2.h>
#include <mem.h>
#include <port.h>
#include <queue.h>

static uint32_t _partial_scancode = 0;
static queue_t _input_queue = (queue_t) { 0, 0, KBD_BUF_SIZE, (void *)KBD_BUF_ADDR};

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

  //TODO: translate!
  *evt = scancode;

  return true;
}
