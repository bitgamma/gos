#ifndef __PIT__
#define __PIT__

#define PIT_CH0_DATA_PORT 0x40
#define PIT_CH2_DATA_PORT 0x42
#define PIT_CMD_PORT 0x43

#define PIT_CH2_GATE_PORT 0x61

#define PIT_CH0_SEL 0x00
#define PIT_CH2_SEL 0x80

#define PIT_ACC_LATCH 0x00
#define PIT_ACC_LO 0x10
#define PIT_ACC_HI 0x20
#define PIT_ACC_LOHI 0x30

#define PIT_MODE_INT 0x00
#define PIT_MODE_ONESHOT 0x02
#define PIT_MODE_RATEGEN 0x04
#define PIT_MODE_SWG 0x06
#define PIT_MODE_SOFT_STROBE 0x08
#define PIT_MODE_HARD_STROBE 0x0A

#define PIT_OSC_HZ 1193182
#define PIT_1MS_DIV (PIT_OSC_HZ / 1000)

void pit_init();

#endif