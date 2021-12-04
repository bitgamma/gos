/**
 * Copyright (c) 2021 Michele Balistreri
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <ps2.h>
#include <port.h>
#include <stdbool.h>
#include <dbg.h>

#define WAIT_CYCLES 255
#define DEV_RETRY 3
#define DEV_INVALID 0xffff

#define PS2_MOUSE_SAMPLE_RATE 40
#define PS2_MOUSE_RESOLUTION PS2_MOUSE_RES_4
#define PS2_MOUSE_SCALING_MODE PS2_MOUSE_LINEAR

uint8_t ps2_mouse_packet_size;

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

  dbg_log_string("ps2: out buffer timed out\n");

  return false;
}

static inline void ps2_send_cmd(uint8_t cmd) {
  outb(PS2_CMD_PORT, cmd);
  ps2_wait_empty_input();
}

static inline void ps2_send_cmd_data(uint8_t cmd, uint8_t data) {
  ps2_send_cmd(cmd);
  outb(PS2_DATA_PORT, data);
  ps2_wait_empty_input();
}

static inline uint8_t ps2_get_cmd_data(uint8_t cmd) {
  outb(PS2_CMD_PORT, cmd);
  ps2_wait_full_output();
  return inb(PS2_DATA_PORT);
}

static bool ps2_dev_cmd(bool port2, uint8_t cmd) {
  uint8_t retry = DEV_RETRY;

  if (port2) {
    ps2_send_cmd(PS2_WRITE_PORT2);
  }

  while(retry--) {
    outb(PS2_DATA_PORT, cmd);

    if (!ps2_wait_full_output()) {
      return false;
    }

    uint8_t ack = inb(PS2_DATA_PORT);

    if (ack == PS2_DEV_ACK) {
      return true;
    } else if (ack != PS2_DEV_NACK) {
      dbg_log_string("ps2: dev nack\n");
      retry = 0;
    }
  }

  return false;
}

static bool ps2_dev_set_data(bool port2, uint8_t cmd, uint8_t data) {
  if (ps2_dev_cmd(port2, cmd)) {
    return ps2_dev_cmd(port2, data);
  }

  return false;
}

static uint16_t ps2_dev_get_data(bool port2, uint8_t cmd) {
  if (ps2_dev_cmd(port2, cmd)) {
    if (ps2_wait_full_output()) {
      return inb(PS2_DATA_PORT);
    }
  }

  return DEV_INVALID;
}

static void ps2_dev_init(bool port2) {
#ifdef PS2_DEV_RESET_ON_INIT
  if (ps2_dev_get_data(port2, PS2_DEV_RESET) != PS2_DEV_TEST_OK) {
    dbg_log_string("ps2: dev reset failed\n");
    return;
  }
#endif
  ps2_dev_cmd(port2, PS2_DEV_DISABLE_SCAN);
  uint16_t dev = ps2_dev_get_data(port2, PS2_DEV_IDENTIFY);

  if (dev == DEV_INVALID || dev == PS2_DEV_KBD) {
    if(ps2_wait_full_output()) {
      inb(PS2_DATA_PORT);
    }

    ps2_dev_set_data(port2, PS2_KBD_SCANCODE, PS2_KBD_SCANCODE2);
    dbg_log_string("ps2: kbd initialized\n");
  } else {
    if (dev != PS2_DEV_STD_MOUSE) {
      ps2_mouse_packet_size = 4;
    } else {
      ps2_mouse_packet_size = 3;
    }

    if (!ps2_dev_set_data(port2, PS2_MOUSE_SET_SAMPLE_RATE, PS2_MOUSE_SAMPLE_RATE)) {
      dbg_log_string("ps2: mouse error setting sampling rate\n");
    };

    if (!ps2_dev_set_data(port2, PS2_MOUSE_SET_RESOLUTION, PS2_MOUSE_RESOLUTION)) {
      dbg_log_string("ps2: mouse error setting resolution\n");
    }

    if (!ps2_dev_cmd(port2, PS2_MOUSE_SCALING_MODE)) {
      dbg_log_string("ps2: mouse error scaling mode\n");
    }

    dbg_log_string("ps2: mouse initialized\n");
  }

  ps2_dev_cmd(port2, PS2_DEV_ENABLE_SCAN);
}

void ps2_init() {
  ps2_send_cmd(PS2_DISABLE_PORT1);
  ps2_send_cmd(PS2_DISABLE_PORT2);

  inb(PS2_DATA_PORT);

  uint8_t ps2_config = ps2_get_cmd_data(PS2_READ_CONFIG) & (~ ((1 << PS2_CCB_PORT1_INT) | (1 << PS2_CCB_PORT2_INT)));
#ifdef PS2_USE_SCANCODE2
  ps2_config &= ~(1 << PS2_CCB_TRANSLATION);
#else
  ps2_config |= (1 << PS2_CCB_TRANSLATION);
#endif

  ps2_send_cmd_data(PS2_WRITE_CONFIG, ps2_config);

  if (ps2_get_cmd_data(PS2_TEST_CONTROLLER) != 0x55) {
    dbg_log_string("ps2: controller test failed\n");
  }

  ps2_send_cmd_data(PS2_WRITE_CONFIG, ps2_config);

  bool port1_working = true;

  if (ps2_get_cmd_data(PS2_TEST_PORT1) != 0x00) {
    dbg_log_string("ps2: port 1 test failed\n");
    port1_working = false;
  } else {
    ps2_config |= (1 << PS2_CCB_PORT1_INT);
  }

  bool port2_working = true;

  if (ps2_get_cmd_data(PS2_TEST_PORT2) != 0x00) {
    dbg_log_string("ps2: port 2 test failed\n");
    port2_working = false;
  } else {
    ps2_config |= (1 << PS2_CCB_PORT2_INT);
  }

  ps2_send_cmd_data(PS2_WRITE_CONFIG, ps2_config);

  if (port1_working) {
    dbg_log_string("ps2: first port enabled\n");
    ps2_send_cmd(PS2_ENABLE_PORT1);
    ps2_dev_init(false);
  }

  if (port2_working) {
    dbg_log_string("ps2: second port enabled\n");
    ps2_send_cmd(PS2_ENABLE_PORT2);
    ps2_dev_init(true);
  }
}

void ps2_reboot() {
  ps2_send_cmd(PS2_REBOOT);
}