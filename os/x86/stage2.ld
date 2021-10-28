ENTRY(_boot)

SECTIONS
{
	. = 512;

	.text BLOCK(4K) : ALIGN(4K)
	{
		*(.text)
    *(.rodata)
    *(.data)
		*(COMMON)
		*(.bss)
	}
}