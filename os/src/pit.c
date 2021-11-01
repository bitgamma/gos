#include <pit.h>
#include <port.h>


void pit_init() {
  outb(PIT_CMD_PORT, PIT_CH0_SEL | PIT_ACC_LOHI | PIT_MODE_SWG);
  outb(PIT_CH0_DATA_PORT, (PIT_1MS_DIV & 0xff));
	outb(PIT_CH0_DATA_PORT, ((PIT_1MS_DIV >> 8) & 0xff));
}