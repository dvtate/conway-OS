ENTRY(kernel_entry)

SECTIONS {
	. = 1M;

	.boot :
	{
		*(.multiboot_hdr)
		boot_stack_start = .;
		. += 0x1000;
		boot_stack_end = .;
	}

	.text :
	{
		*(.text)
	}
}