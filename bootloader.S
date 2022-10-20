####
# Primitive assembly bootloader
####

# Start in 16-bit real mode
.section .text
.code16

# Export start symbol
.globl _start
_start:

# Configure stack and segment regs
# We know that mbr is at 0x7c00
movw $0x7c0, %ax
addw $0x20, %ax
movw %ax, %ss
movw $0x1000, %sp

# Set up data segment
movw $0x7c0, %ax
movw %ax, %ds

# Prepare bios print call
.set OFFSET, my_msg - _start
movw $OFFSET, %si
movb $0xe, %ah
mov $0, %dx

print_loop:
	lodsb
	cmp $0, %al
	je done
	int $0x10
	jmp print_loop

done:
	hlt
	jmp done

my_msg:
	.asciz "Welcome to Conway's Game of Life!\n"

# Make it clear this is a MBR
remainder:
	.set MBR_REMAINDER, 510 - (remainder - _start)
	.fill MBR_REMAINDER, 1, 0x0
	.word 0xaa55
