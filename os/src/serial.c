/**
 * Copyright (c) 2021 Michele Balistreri
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <serial.h>
#include <port.h>
#include <queue.h>

#define SERIAL_DLAB 0
#define SERIAL_IER 1
#define SERIAL_IIR 2
#define SERIAL_FCR 2
#define SERIAL_LCR 3
#define SERIAL_MCR 4
#define SERIAL_LSR 5
#define SERIAL_MSR 6

#define SERIAL_IER_RECV (1 << 0)
#define SERIAL_IER_SEND (1 << 1)

#define SERIAL_LCR_8BIT 3
#define SERIAL_LCR_STOP_ONE 0
#define SERIAL_LCR_PARITY_NONE 0
#define SERIAL_LCR_BREAK_NONE 0
#define SERIAL_LCR_DLAB 0x80

#define SERIAL_FCR_ENABLE (1 << 0)
#define SERIAL_FCR_CLEAR (3 << 1)
#define SERIAL_FCR_THRESHOLD1 0
#define SERIAL_FCR_THRESHOLD4 (1 << 6)
#define SERIAL_FCR_THRESHOLD8 (2 << 6)
#define SERIAL_FCR_THRESHOLD14 (3 << 6)

#define SERIAL_MCR_DTR (1 << 0)
#define SERIAL_MCR_RTS (1 << 1)
#define SERIAL_MCR_AUXOUT (3 << 2)
#define SERIAL_MCR_LOOPBACK (1 << 4)

#define SERIAL_LSR_DR (1 << 0)
#define SERIAL_LSR_EDHR (1 << 5)

#ifndef SERIAL_RECV_BUF
#define SERIAL_RECV_BUF 16
#endif
#ifndef SERIAL_SEND_BUF
#define SERIAL_SEND_BUF 16
#endif
#ifndef SERIAL_PORT_COUNT
#define SERIAL_PORT_COUNT 0
#endif

typedef struct {
  uint16_t port;
  bool rcv_rdy;
  queue_t recv;
  queue_t send;
} serial_port_t;

static uint8_t _serial_buf[(SERIAL_RECV_BUF + SERIAL_SEND_BUF) * SERIAL_PORT_COUNT];
static serial_port_t _ports[SERIAL_PORT_COUNT];

void serial_init() {
  for (int i = 0; i < SERIAL_PORT_COUNT; i++) {
    _ports[i].recv.buffer = &_serial_buf[(SERIAL_RECV_BUF + SERIAL_SEND_BUF) * i];
    _ports[i].recv.size = SERIAL_RECV_BUF;

    _ports[i].send.buffer = &_serial_buf[((SERIAL_RECV_BUF + SERIAL_SEND_BUF) * i) + SERIAL_RECV_BUF];
    _ports[i].send.size = SERIAL_SEND_BUF;
  }
}

serial_desc_t serial_open(uint16_t port, serial_baud_t rate) {
  serial_desc_t desc;

  for (desc = 0; desc < SERIAL_PORT_COUNT; desc++) {
    if (!_ports[desc].port) {
      _ports[desc].port = port;
      break;
    }
  }

  if (desc == SERIAL_PORT_COUNT) {
    return SERIAL_NODESC;
  }

  outb(port+SERIAL_IER, 0);
  outb(port+SERIAL_LCR, SERIAL_LCR_DLAB);
  outb(port+SERIAL_DLAB, (rate & 0xff));
  outb(port+SERIAL_IER, (rate >> 8));
  outb(port+SERIAL_LCR, SERIAL_LCR_8BIT | SERIAL_LCR_PARITY_NONE | SERIAL_LCR_BREAK_NONE);
  outb(port+SERIAL_FCR, SERIAL_FCR_ENABLE | SERIAL_FCR_CLEAR | SERIAL_FCR_THRESHOLD8);
  outb(port+SERIAL_MCR, SERIAL_MCR_DTR | SERIAL_MCR_RTS | SERIAL_MCR_AUXOUT);
  outb(port+SERIAL_IER, SERIAL_IER_RECV);

  _ports[desc].recv.count = 0;
  _ports[desc].send.count = 0;
  _ports[desc].rcv_rdy = false;

  return desc;
}

void serial_close(serial_desc_t sd) {
  if (!_ports[sd].port) {
    return;
  }

  outb(_ports[sd].port+SERIAL_IER, 0);
  outb(_ports[sd].port+SERIAL_FCR, SERIAL_FCR_CLEAR);

  _ports[sd].port = 0;
}

bool serial_read(serial_desc_t sd, uint8_t* b) {
  return queue_read_circular_uint8(&_ports[sd].recv, b);
}

bool serial_write(serial_desc_t sd, uint8_t b) {
  return queue_push_circular_uint8(&_ports[sd].send, b);
}

void serial_recv(uint16_t port) {
  for (int i = 0; i < SERIAL_PORT_COUNT; i++) {
    if (_ports[i].port == port) {
      _ports[i].rcv_rdy = true;
      queue_push_circular_uint8(&_ports[i].recv, inb(port));
      break;
    }
  }
}

static inline bool serial_has_data(uint16_t port) {
  return inb(port+SERIAL_LSR) & SERIAL_LSR_DR;
}

static inline bool serial_send_empty(uint16_t port) {
  return inb(port+SERIAL_LSR) & SERIAL_LSR_EDHR;
}

__attribute__((always_inline)) static inline void serial_run_port(serial_port_t* serial) {
  if (serial->rcv_rdy) {
    serial->rcv_rdy = false;
    while(serial_has_data(serial->port)) {
      queue_push_circular_uint8(&serial->recv, inb(serial->port));
    }
  }

  while (serial->send.count) {
    if (!serial_send_empty(serial->port)) {
      break;
    }

    uint8_t b;
    queue_read_circular_uint8(&serial->send, &b);
    outb(serial->port, b);
  }
}

void serial_run() {
  for (int i = 0; i < SERIAL_PORT_COUNT; i++) {
    if (_ports[i].port) {
      serial_run_port(&_ports[i]);
    }
  }
}
