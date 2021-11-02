#include <cmos.h>
#include <port.h>

uint8_t _nmi;

uint8_t cmos_read(uint8_t reg) {
  outb(CMOS_CMD_PORT, _nmi | reg);
  return inb(CMOS_DATA_PORT);
}

void cmos_write(uint8_t reg, uint8_t data) {
  outb(CMOS_CMD_PORT, reg);
  outb(CMOS_DATA_PORT, data);
}

void cmos_nmi_enable() {
  _nmi = 0;
  outb(CMOS_CMD_PORT, inb(CMOS_CMD_PORT) & 0x7F);
  inb(CMOS_DATA_PORT);
}

void cmos_nmi_disable() {
  _nmi = 0x80;
  outb(CMOS_CMD_PORT, inb(CMOS_CMD_PORT) | 0x80);
  inb(CMOS_DATA_PORT);
}