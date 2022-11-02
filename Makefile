CC:=g++
AS:=as
LD:=ld
QEMU:=qemu-system-i386

#QEMUOPTS:=-nographic
#QEMUOPTS:=-vga std

KERN_LINK_SCRIPT:=kernel.ld

KERN_CFLAGS:= -ffreestanding -nostdlib -m32 -fno-exceptions -fno-rtti -fno-stack-protector
KERN_LDFLAGS:= -T $(KERN_LINK_SCRIPT) -m elf_i386

bootloader.bin: bootloader.o
	$(LD) -m elf_i386 -Ttext 0x7c00 --oformat binary -o $@ $<

boot.o: boot.S
	$(CC) $(KERN_CFLAGS) -o $@ -c $<

kernel.o: kernel.cpp
	$(CC) $(KERN_CFLAGS) -o $@ -c $<

kern.bin: boot.o kernel.o
	$(LD) $(KERN_LDFLAGS) -o $@ kernel.o boot.o

iso: kern.bin
	@cp kern.bin cdrom/boot/
	@grub-mkrescue -o kern.iso cdrom

bootloader.o: bootloader.S
	$(AS) --32 -o $@ $<

all: bootloader.bin kern.bin iso

dump_bl: bootloader.bin
	@objdump -D -b binary -m i8086 $<

run-bl: bootloader.bin
	$(QEMU) -vga std -drive file=bootloader.bin,index=0,media=disk,format=raw $(QEMUOPTS)
run-iso: iso
	$(QEMU) -cdrom kern.iso

clean:
	@rm -f *.bin *.o *.log *.iso cdrom/boot/*.bin
