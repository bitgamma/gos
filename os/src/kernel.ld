ENTRY(kernel_main)

SECTIONS
{
	. = 0x100000;

	.text BLOCK(4K) : ALIGN(4K)
	{
		*(.text)
    *(.rodata)
    *(.data)
	}
}