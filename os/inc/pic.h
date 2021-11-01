#ifndef __PIC__
#define __PIC__

#include <stdint.h>
#include <stdbool.h>

#define PIC1_COMMAND	0x20
#define PIC1_DATA	0x21

#define PIC2_COMMAND 0xA0
#define PIC2_DATA 0xA1

#define PIC_EOI 0x20

#define PIC_READ_IRR 0x0a
#define PIC_READ_ISR 0x0b

#define PIC1_IRQ_OFFSET 0x20
#define PIC2_IRQ_OFFSET 0x28

#define ICW1_ICW4	0x01
#define ICW1_SINGLE	0x02
#define ICW1_INTERVAL4 0x04
#define ICW1_LEVEL 0x08
#define ICW1_INIT	0x10

#define ICW4_8086	0x01
#define ICW4_AUTO	0x02
#define ICW4_BUF_SLAVE 0x08
#define ICW4_BUF_MASTER	0x0C
#define ICW4_SFNM	0x10

#define PIC_TIMER 0
#define PIC_KEYBOARD 1
#define PIC_CASCADE 2
#define PIC_COM2 3
#define PIC_COM1 4
#define PIC_LPT2 5
#define PIC_FLOPPY 6
#define PIC_LPT1 7
#define PIC_CMOS_RTC 8
#define PIC_PERIPHERAL1 9
#define PIC_PERIPHERAL2 10
#define PIC_PERIPHERAL3 11
#define PIC_MOUSE 12
#define PIC_FPU 13
#define PIC_ATA1 14
#define PIC_ATA2 15

#define DEFAULT_IRQ (~((1 << PIC_TIMER) | (1 << PIC_KEYBOARD) | (1 << PIC_CASCADE) | (1 << PIC_LPT1) | (1 << PIC_CMOS_RTC) | (1 << PIC_ATA1) | (1 << PIC_ATA2)))

#define PIC_IRQ(a) (PIC1_IRQ_OFFSET + a)

void pic_eoi(uint8_t irq);
void pic_init();
void pic_enable_irq(uint8_t irq);
void pic_disable_irq(uint8_t irq);
void pic_disable();
bool pic_is_spurious(uint8_t irq);
uint16_t pic_get_irr(void);
uint16_t pic_get_isr(void);

#endif