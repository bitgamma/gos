#ifndef __INTERRUPT__
#define __INTERRUPT__

#include <stdint.h>
#include <mem.h>


#define IDT_MAX_DESCRIPTORS 256

#define IDTR ((idtr_t *)IDTR_ADDR)
#define IDT ((idt_entry_t *) IDT_ADDR)

typedef struct __attribute__((packed)) {
	uint16_t isr_low;
	uint16_t kernel_cs;
	uint8_t reserved;
	uint8_t attributes;
	uint16_t isr_high;
} idt_entry_t;

typedef struct __attribute__((packed)) {
	uint16_t	limit;
	uint32_t	base;
} idtr_t;

inline void idt_enable() {
	asm volatile ("sti");
}

inline void idt_disable() {
	asm volatile ("cli");
}

void idt_set_descriptor(uint8_t vector, void* isr, uint8_t flags);
void idt_init(void);

#endif
