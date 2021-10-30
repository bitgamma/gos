ENTRY(_stage2)

SECTIONS
{
	. = 0x7E00;

	.text :
	{
		*(.text)
    *(.rodata)
    *(.data)
	}
}