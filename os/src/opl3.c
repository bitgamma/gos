#include <opl3.h>
#include <port.h>
#include <dbg.h>

static opl3_type_t _type;
static uint16_t _base_port;

static void opl3_cmd(uint16_t port, uint8_t reg, uint8_t data, bool slow) {
  outb(port, reg);
  iowait();

  if (slow) {
    iowait();
    iowait();
  }

  outb(port + 1, data);
  iowait();

  if (slow) {
    for (uint8_t i = 0; i < 20; i++) {
      iowait();
    }
  }
}

static opl3_type_t opl3_detect(uint16_t port) {
  opl3_cmd(port, OPL3_TIMER_CR, (OPL3_TIMER1_MASK | OPL3_TIMER2_MASK), true);
  opl3_cmd(port, OPL3_TIMER_CR, OPL3_IRQ_RESET, true);

  uint8_t signature = inb(port);

  if ((signature & 0xe0) != 0x00) {
    return NONE;
  }

  opl3_cmd(port, OPL3_TIMER1, 0xff, true);
  opl3_cmd(port, OPL3_TIMER_CR, (OPL3_TIMER2_MASK | OPL3_TIMER1_START), true);

  for (uint8_t i = 0; i < 80; i++) {
    iowait();
  }

  uint8_t status = inb(port);

  opl3_cmd(port, OPL3_TIMER_CR, (OPL3_TIMER1_MASK | OPL3_TIMER2_MASK), true);
  opl3_cmd(port, OPL3_TIMER_CR, OPL3_IRQ_RESET, true);

  if ((status & 0xe0) != 0xc0) {
      return NONE;
  }

  return signature == 0x06 ? OPL2 : OPL3;
}

opl3_type_t opl3_init() {
  _type = opl3_detect(SB_BASE_PORT);

  if (_type == OPL2) {
    _base_port = SB_BASE_PORT;
    if (opl3_detect(SB_BASE_PORT + 2) != NONE) {
      _type = DUAL_OPL2;
    }
  } else if (_type == OPL3) {
    _base_port = SB_BASE_PORT;
  } else if (_type == NONE) {
    _type = opl3_detect(ADLIB_BASE_PORT);
    _base_port = ADLIB_BASE_PORT;
  }

  dbg_log_string("opl3: detected type: ");
  dbg_log_uint8(_type);
  return _type;
}

opl3_type_t opl3_get_type() {
  return _type;
}

void opl3_write(uint16_t reg, uint8_t data) {
  bool slow = _type != OPL3;

  if (reg & 0x100) {
    if (_type != OPL2) {
      opl3_cmd(_base_port + 2, reg & 0xff, data, slow);
    }
  } else {
    opl3_cmd(_base_port, reg, data, slow);
  }
}