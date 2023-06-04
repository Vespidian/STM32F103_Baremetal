stack_ptr_init = 0x20005000;

MEMORY{
	FLASH 	(rx)	: ORIGIN = 0x08000000, LENGTH = 64k
	RAM		(rxw)	: ORIGIN = 0x20000000, LENGTH = 20k
}

SECTIONS{
	. = 0x0;

	/*	Vector table should be at the start of flash */
	.vector_table : {
		*(vector_table)
	} > FLASH

	/*	Contains the main program code */
	.text : {
		*(.text)
	} > FLASH

	/*	Contains read-only data.
		constants, strings, and other unchanging information */
	.rodata : {
		. = ALIGN(4);
		*(.rodata)
		*(.rodata*)
		. = ALIGN(4);
	} > FLASH

	/*	Space set aside for variables and other changing information */
	.data : {
		. = ALIGN(4);
		_data_s = .;
		*(.data)
		*(.data*)
		_data_e = .;
		. = ALIGN(4);
		_data_flash_addr = LOADADDR(.data);
		
	} > RAM AT > FLASH

	/*	Similar to 'data' section except, it is initialized to all 0s */
	.bss : {
		_bss_s = .;
		*(.bss)
		_bss_e = .;
	} > RAM
}