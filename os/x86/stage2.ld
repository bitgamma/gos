ENTRY(_boot)

SECTIONS
{
	. = 1M;

	.text BLOCK(4K) : ALIGN(4K)
	{
		*(.text)
    *(.rodata)
    *(.data)
		*(COMMON)
		*(.bss)
	}
}