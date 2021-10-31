#include <pic.h>
#include <port.h>

void pic_eoi(uint8_t irq) {
	if(irq >= 8) {
    outb(PIC2_COMMAND, PIC_EOI);
  }

	outb(PIC1_COMMAND, PIC_EOI);
}

void pic_init() {
	outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
	iowait();
	outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
	iowait();

	outb(PIC1_DATA, PIC1_IRQ_OFFSET);
	iowait();
	outb(PIC2_DATA, PIC2_IRQ_OFFSET);
	iowait();
	outb(PIC1_DATA, 4);
	iowait();
	outb(PIC2_DATA, 2);
	iowait();

	outb(PIC1_DATA, ICW4_8086);
	iowait();
	outb(PIC2_DATA, ICW4_8086);
	iowait();

	outb(PIC1_DATA, (DEFAULT_IRQ & 0xff));
	outb(PIC2_DATA, ((DEFAULT_IRQ >> 8) & 0xff));
}

void pic_enable_irq(uint8_t irq) {
  uint16_t port;
  uint8_t value;

  if (irq < 8) {
    port = PIC1_DATA;
  } else {
    port = PIC2_DATA;
    irq -= 8;
  }

  value = inb(port) & ~(1 << irq);
  outb(port, value);
}

void pic_disable_irq(uint8_t irq) {
  uint16_t port;
  uint8_t value;

  if (irq < 8) {
    port = PIC1_DATA;
  } else {
    port = PIC2_DATA;
    irq -= 8;
  }

  value = inb(port) | (1 << irq);
  outb(port, value);
}

void pic_disable() {
  outb(PIC2_DATA, 0xff);
  outb(PIC1_DATA, 0xff);
}

bool pic_is_spurious(uint8_t irq) {
  if (irq == PIC_ATA2) {
    outb(PIC2_COMMAND, PIC_READ_ISR);

    if ((inb(PIC2_COMMAND) & 0x80) == 0) {
      pic_eoi(PIC_CASCADE);
      return true;
    }
  } else if (irq == PIC_LPT1) {
    outb(PIC1_COMMAND, PIC_READ_ISR);

    if ((inb(PIC1_COMMAND) & 0x80) == 0) {
      return true;
    }
  }

  return false;
}

static uint16_t __pic_get_irq_reg(int ocw3) {
  outb(PIC1_COMMAND, ocw3);
  outb(PIC2_COMMAND, ocw3);
  return (inb(PIC2_COMMAND) << 8) | inb(PIC1_COMMAND);
}

uint16_t pic_get_irr(void) {
  return __pic_get_irq_reg(PIC_READ_IRR);
}

uint16_t pic_get_isr(void) {
  return __pic_get_irq_reg(PIC_READ_ISR);
}