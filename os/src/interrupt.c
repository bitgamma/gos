#include <interrupt.h>
#include <mem.h>
#include <stdbool.h>

typedef unsigned int uword_t;
struct interrupt_frame;

__attribute__((interrupt)) void exception_handler(__attribute__ ((unused)) struct interrupt_frame* frame) {
	asm volatile ("cli\n" "hlt");
}

__attribute__ ((interrupt)) void error_handler(__attribute__ ((unused)) struct interrupt_frame *frame, __attribute__ ((unused)) uword_t error_code) {
	asm volatile ("cli\n" "hlt");
}

__attribute__((aligned(0x10))) static idt_entry_t IDT[256];

void idt_set_descriptor(uint8_t vector, void* isr, uint8_t flags) {
  idt_entry_t* descriptor = &IDT[vector];

  descriptor->isr_low = (uint32_t)isr & 0xffff;
  descriptor->kernel_cs = KERNEL_GDT_ENTRY;
  descriptor->attributes = flags;
  descriptor->isr_high = (uint32_t)isr >> 16;
  descriptor->reserved = 0;
}

void idt_init() {
  IDTR->base = (uint32_t)&IDT[0];
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

  asm volatile ( "lidt %0\n" : : "m"(*IDTR));
}
