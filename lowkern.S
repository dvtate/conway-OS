/*
* Multiboot header
*/
.section .multiboot_hdr
.set MULTIBOOT_MAGIC, 0xe85250d6
.set ARCH, 0x0

multiboot_hdr:
	.long MULTIBOOT_MAGIC
	.long ARCH
	.long hdr_end - multiboot_hdr
	.long -(MULTIBOOT_MAGIC + (hdr_end - multiboot_hdr))

	/* no special tags */
	.word 0, 0
	.long 8

hdr_end:


# Kernel
.section .text
.code32
.globl kernel_entry
kernel_entry:
	movb $'O',  0xb8000
	movb $0x0f, 0xb8001
	movb $'K',  0xb8002
	movb $0x0f, 0xb8003

end:
	hlt
	jmp end
