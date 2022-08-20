stack_ptr_init = 0x20005000;

MEMORY{
	FLASH 	(rx)	: ORIGIN = 0x08000000, LENGTH = 64k
	RAM		(rxw)	: ORIGIN = 0x20000000, LENGTH = 20k
}

SECTIONS{
	. = 0x0;

	.text : {
		*(vector_table)
		*(.text)
	}
}