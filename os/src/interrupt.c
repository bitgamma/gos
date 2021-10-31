#include <interrupt.h>
#include <mem.h>
#include <stdbool.h>

extern void* isr_stub_table[];
extern void load_idtr_and_enable();

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

  for (uint8_t vector = 0; vector < 32; vector++) {
    idt_set_descriptor(vector, isr_stub_table[vector], 0x8e);
  }

  load_idtr_and_enable();
}