/**
 * Copyright (c) 2021 Michele Balistreri
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef __SERIAL__
#define __SERIAL__

#include <stdint.h>
#include <stdbool.h>
#include <config.h>

#define SERIAL_COM1 0x3f8
#define SERIAL_COM2 0x2f8
#define SERIAL_COM3 0x3e8
#define SERIAL_COM4 0x2e8

#define SERIAL_NODESC -1

typedef enum {
  BAUD_115200 = 1,
  BAUD_57600 = 2,
  BAUD_38400 = 3,
  BAUD_19200 = 6,
  BAUD_9600 = 12,
} serial_baud_t;

typedef int8_t serial_desc_t;

void serial_recv(uint16_t port);
void serial_init();
serial_desc_t serial_open(uint16_t port, serial_baud_t rate);
void serial_close(serial_desc_t port);
bool serial_read(serial_desc_t sd, uint8_t* b);
bool serial_write(serial_desc_t sd, uint8_t b);

void serial_run();

#endif