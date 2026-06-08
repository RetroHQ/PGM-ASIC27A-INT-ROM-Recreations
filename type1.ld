OUTPUT_FORMAT(elf32-littlearm)
ENTRY(start)

MEMORY
{
	ROM(rx) : ORIGIN = 0, LENGTH = 0x4000
	RAM(rw) : ORIGIN = 0x10000000, LENGTH = 0x400
}

SECTIONS
{
	.text :
	{
		*(.startup .startup.*)
		*(.text .text.*)
		*(.rtext .rtext.*)
		*(.rodata .rodata.*)
		. = ALIGN(4);
	} >ROM

	.data :
	{
		__data_start = .;
		*(.data .data.*)
		*(.rdata .rdata.*)
		. = ALIGN(4);
		__data_end = .;
	} >RAM AT >ROM
	
	.bss :
	{
		__bss_start = .;
		*(.bss COMMON .bss.*)
		. = ALIGN(4);
		__bss_end = .;
	} >RAM

	__data_init = LOADADDR(.data);

	__data_size = __data_end - __data_start;
	__bss_size = __bss_end - __bss_start;

	/DISCARD/ :
	{
		*(.comment)
		*(.debug*)
	}
}
