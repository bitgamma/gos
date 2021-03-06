/**
 * Copyright (c) 2021 Michele Balistreri
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <interrupt.h>
#include <mem.h>
#include <pic.h>
#include <port.h>
#include <stdbool.h>
#include <kbd.h>
#include <mouse.h>
#include <timer.h>
#include <sb16.h>
#include <serial.h>
#include <dbg.h>

#define KERNEL_GDT_ENTRY 0x08

typedef unsigned int uword_t;
struct interrupt_frame;

__attribute__((interrupt)) void exception_handler(__attribute__ ((unused)) struct interrupt_frame* frame) {
  asm volatile ("nop");
}

__attribute__ ((interrupt)) void error_handler(__attribute__ ((unused)) struct interrupt_frame *frame, __attribute__ ((unused)) uword_t error_code) {
  asm volatile ("nop");
}

__attribute__ ((interrupt)) void timer_handler(__attribute__ ((unused)) struct interrupt_frame *frame) {
  SYSTICK += TIMER_RES_MS;
  pic_eoi(PIC_TIMER);
}

__attribute__ ((interrupt)) void keyboard_handler(__attribute__ ((unused)) struct interrupt_frame *frame) {
  kbd_ps2_rcv();
  pic_eoi(PIC_KEYBOARD);
}

__attribute__ ((interrupt)) void com2_handler(__attribute__ ((unused)) struct interrupt_frame *frame) {
  serial_recv(SERIAL_COM2);
  pic_eoi(PIC_COM2);
}

__attribute__ ((interrupt)) void com1_handler(__attribute__ ((unused)) struct interrupt_frame *frame) {
  serial_recv(SERIAL_COM1);
  pic_eoi(PIC_COM1);
}

__attribute__ ((interrupt)) void sb16_handler(__attribute__ ((unused)) struct interrupt_frame *frame) {
  sb16_transfer_finished();
  pic_eoi(PIC_LPT2);
}

__attribute__ ((interrupt)) void lpt1_handler(__attribute__ ((unused)) struct interrupt_frame *frame) {
  if (!pic_is_spurious(PIC_LPT1)) {
    pic_eoi(PIC_LPT1);
  } else {
    dbg_log_string("irq: spurious IRQ7\n");
  }
}

__attribute__ ((interrupt)) void rtc_handler(__attribute__ ((unused)) struct interrupt_frame *frame) {
  pic_eoi(PIC_CMOS_RTC);
}

__attribute__ ((interrupt)) void mouse_handler(__attribute__ ((unused)) struct interrupt_frame *frame) {
  mouse_ps2_rcv();
  pic_eoi(PIC_MOUSE);
}

__attribute__ ((interrupt)) void ata1_handler(__attribute__ ((unused)) struct interrupt_frame *frame) {
  pic_eoi(PIC_ATA1);
}

__attribute__ ((interrupt)) void ata2_handler(__attribute__ ((unused)) struct interrupt_frame *frame) {
  if (!pic_is_spurious(PIC_ATA2)) {
    pic_eoi(PIC_ATA2);
  } else {
    dbg_log_string("irq: spurious IRQ15\n");
  }
}

void idt_set_descriptor(uint8_t vector, void* isr, uint8_t flags) {
  idt_entry_t* descriptor = &IDT[vector];

  descriptor->isr_low = (uint32_t)isr & 0xffff;
  descriptor->kernel_cs = KERNEL_GDT_ENTRY;
  descriptor->attributes = flags;
  descriptor->isr_high = (uint32_t)isr >> 16;
  descriptor->reserved = 0;
}

void idt_init() {
  IDTR->base = IDT_ADDR;
  IDTR->limit = (uint16_t) ((sizeof(idt_entry_t) * IDT_MAX_DESCRIPTORS) - 1);

  for (uint8_t vector = 0; vector < 8; vector++) {
    idt_set_descriptor(vector, exception_handler, 0x8e);
  }

  idt_set_descriptor(8, error_handler, 0x8e);
  idt_set_descriptor(9, exception_handler, 0x8e);

  for (uint8_t vector = 10; vector < 15; vector++) {
    idt_set_descriptor(vector, error_handler, 0x8e);
  }

  idt_set_descriptor(15, exception_handler, 0x8e);
  idt_set_descriptor(16, exception_handler, 0x8e);
  idt_set_descriptor(17, error_handler, 0x8e);

  for (uint8_t vector = 18; vector < 30; vector++) {
    idt_set_descriptor(vector, exception_handler, 0x8e);
  }

  idt_set_descriptor(30, exception_handler, 0x8e);
  idt_set_descriptor(31, error_handler, 0x8e);

  idt_set_descriptor(PIC_IRQ(PIC_TIMER), timer_handler, 0x8e);
  idt_set_descriptor(PIC_IRQ(PIC_KEYBOARD), keyboard_handler, 0x8e);
  idt_set_descriptor(PIC_IRQ(PIC_COM2), com2_handler, 0x8e);
  idt_set_descriptor(PIC_IRQ(PIC_COM1), com1_handler, 0x8e);
  idt_set_descriptor(PIC_IRQ(PIC_LPT2), sb16_handler, 0x8e);
  idt_set_descriptor(PIC_IRQ(PIC_LPT1), lpt1_handler, 0x8e);
  idt_set_descriptor(PIC_IRQ(PIC_CMOS_RTC), rtc_handler, 0x8e);
  idt_set_descriptor(PIC_IRQ(PIC_MOUSE), mouse_handler, 0x8e);
  idt_set_descriptor(PIC_IRQ(PIC_ATA1), ata1_handler, 0x8e);
  idt_set_descriptor(PIC_IRQ(PIC_ATA2), ata2_handler, 0x8e);

  asm volatile ( "lidt %0\n" : : "m"(*IDTR));
}
