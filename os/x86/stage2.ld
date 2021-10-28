ENTRY(_stage2)

SECTIONS
{
	. = 0x7E00;

	.text BLOCK(4K) : ALIGN(4K)
	{
		*(.text)
    *(.rodata)
    *(.data)
		*(COMMON)
		*(.bss)
	}
}