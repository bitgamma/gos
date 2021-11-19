/**
 * Copyright (c) 2021 Michele Balistreri
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

ENTRY(_kstart)

SECTIONS {
	. = 0x100000;

	.text : {
		*(.text.startup)
		*(.text)
    *(.rodata*)
    *(.data)
	}

	. = 0x10000;

	.bss : {
		*(.bss)
		*(COMMON)
	}
}