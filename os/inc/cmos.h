/**
 * Copyright (c) 2021 Michele Balistreri
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef __CMOS__
#define __CMOS__

#include <stdint.h>

#define CMOS_CMD_PORT 0x70
#define CMOS_DATA_PORT 0x71

#define CMOS_RTC_SECONDS 0x00
#define CMOS_RTC_MINUTES 0x02
#define CMOS_RTC_HOURS 0x04
#define CMOS_RTC_WEEKDAY 0x06
#define CMOS_RTC_DAY 0x07
#define CMOS_RTC_MONTH 0x08
#define CMOS_RTC_YEAR 0x09
#define CMOS_RTC_CENTURY 0x32
#define CMOS_RTC_STATUS_REG_A 0x0A
#define CMOS_RTC_STATUS_REG_B 0x0B
#define CMOS_RTC_STATUS_REG_C 0x0C

uint8_t cmos_read(uint8_t reg);
void cmos_write(uint8_t reg, uint8_t data);

void nmi_enable();
void nmi_disable();

#endif