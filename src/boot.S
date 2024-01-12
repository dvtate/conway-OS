# Multiboot header
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


# Stack
.section .bss
.align 16
stack_bottom:
.skip 65536 # 64 KiB
stack_top:


# Kernel
.section .text
.code32
.globl kernel_entry
kernel_entry:
	# Initialize stack
	mov $stack_top, %esp

	# Jump to C++
    call bootmain

end:
	hlt
	jmp end
