#include <ps2.h>
#include <port.h>
#include <stdbool.h>

#define WAIT_CYCLES 5
#define KBD_RETRY 3
#define KBD_INVALID 0xffff

static void ps2_wait_empty_input() {
  while(inb(PS2_CMD_PORT) & (1 << PS2_STS_INFULL)) {
    asm volatile ("nop");
  }
}

static bool ps2_wait_full_output() {
  uint8_t retry = WAIT_CYCLES;

  while(retry--) {
    if (inb(PS2_CMD_PORT) & (1 << PS2_STS_OUTFULL)) {
      return true;
    }

    iowait();
  }

  return false;
}

static inline void ps2_send_cmd_data(uint8_t cmd, uint8_t data) {
  outb(PS2_CMD_PORT, cmd);
  ps2_wait_empty_input();
  outb(PS2_DATA_PORT, data);
}

static inline uint8_t ps2_get_cmd_data(uint8_t cmd) {
  outb(PS2_CMD_PORT, cmd);
  ps2_wait_full_output();
  return inb(PS2_DATA_PORT);
}

static bool ps2_kbd_cmd(uint8_t cmd) {
  uint8_t retry = KBD_RETRY;

  while(retry--) {
    outb(PS2_DATA_PORT, cmd);

    if (!ps2_wait_full_output()) {
      return false;
    }

    if (inb(PS2_DATA_PORT) & 0xFA) {
      return true;
    }
  }

  return false;
}

static bool ps2_kbd_set_data(uint8_t cmd, uint8_t data) {
  uint8_t retry = KBD_RETRY;

  while(retry--) {
    outb(PS2_DATA_PORT, cmd);
    ps2_wait_empty_input();
    outb(PS2_DATA_PORT, data);

    if (!ps2_wait_full_output()) {
      return false;
    }

    if (inb(PS2_DATA_PORT) & 0xFA) {
      return true;
    }
  }

  return false;
}

static uint16_t ps2_kbd_get_data(uint8_t cmd) {
  if (ps2_kbd_cmd(cmd)) {
    if (ps2_wait_full_output()) {
      return inb(PS2_DATA_PORT);
    }
  }

  return KBD_INVALID;
}

static void ps2_kbd_init() {
  if (ps2_kbd_get_data(PS2_KBD_RESET) != PS2_KBD_TEST_OK) {
    // do something
    return;
  }

  ps2_kbd_cmd(PS2_KBD_DISABLE_SCAN);
  ps2_kbd_set_data(PS2_KBD_SCANCODE, PS2_KBD_SCANCODE2);
  ps2_kbd_cmd(PS2_KBD_ENABLE_SCAN);
}

void ps2_init() {
  outb(PS2_CMD_PORT, PS2_DISABLE_PORT1);
  outb(PS2_CMD_PORT, PS2_DISABLE_PORT2);

  inb(PS2_DATA_PORT);

  uint8_t ps2_config = ps2_get_cmd_data(PS2_READ_CONFIG) & (~ ((1 << PS2_CCB_PORT1_INT) | (1 << PS2_CCB_PORT2_INT) | (1 << PS2_CCB_TRANSLATION)));
  ps2_send_cmd_data(PS2_WRITE_CONFIG, ps2_config);

  if (ps2_get_cmd_data(PS2_TEST_CONTROLLER) != 0x55) { /*do something*/ }
  ps2_send_cmd_data(PS2_WRITE_CONFIG, ps2_config);

  if (ps2_get_cmd_data(PS2_TEST_PORT1) != 0x00) { /*do something*/ }

  outb(PS2_CMD_PORT, PS2_ENABLE_PORT1);
  ps2_config |= (1 << PS2_CCB_PORT1_INT);
  ps2_send_cmd_data(PS2_WRITE_CONFIG, ps2_config);

  ps2_kbd_init();
}

void ps2_reboot() {
  ps2_wait_empty_input();
  outb(PS2_CMD_PORT, PS2_REBOOT);
}