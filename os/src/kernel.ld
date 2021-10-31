ENTRY(_kstart)

SECTIONS {
	. = 0x100000;

	.text : {
		*(.text.startup)
		*(.text)
    *(.rodata*)
    *(.data)
	}

	. = 0x30000;

	.bss : {
		*(.bss)
		*(COMMON)
	}
}