#ifndef __OPL3__
#define __OPL3__

#include <stdint.h>
#include <stdbool.h>

typedef enum __attribute__((packed)) { NONE, OPL2, DUAL_OPL2, OPL3 } opl3_type_t;

#define ADLIB_BASE_PORT 0x388
#define SB_BASE_PORT 0x220

#define OPL3_TIMER1 0x02
#define OPL3_TIMER2 0x03
#define OPL3_TIMER_CR	0x04
#define OPL3_IRQ_RESET 0x80
#define OPL3_TIMER1_MASK 0x40
#define OPL3_TIMER2_MASK 0x20
#define OPL3_TIMER1_START	0x01

opl3_type_t opl3_init();
opl3_type_t opl3_get_type();
void opl3_write(uint16_t reg, uint8_t data);

#endif