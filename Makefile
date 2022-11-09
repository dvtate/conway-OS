CC:=g++
AS:=as
LD:=ld
QEMU:=qemu-system-i386

#QEMUOPTS:=-nographic
#QEMUOPTS:=-vga std

KERN_LINK_SCRIPT:=kernel.ld

KERN_CFLAGS:= -ffreestanding -nostdlib -m32 -fno-exceptions -fno-rtti -fno-stack-protector
KERN_LDFLAGS:= -T $(KERN_LINK_SCRIPT) -m elf_i386

boot.o: src/boot.S
	$(CC) $(KERN_CFLAGS) -o $@ -c $<

kernel.o: src/kernel.cpp
	$(CC) $(KERN_CFLAGS) -o $@ -c $<
crt.o: src/crt.cpp
	$(CC) $(KERN_CFLAGS) -o $@ -c $<

kern.bin: boot.o kernel.o crt.o
	$(LD) $(KERN_LDFLAGS) -o $@ kernel.o boot.o crt.o

iso: kern.bin
	@cp kern.bin cdrom/boot/
	@grub-mkrescue -o kern.iso cdrom

all: kern.bin iso

dump: kern.bin
	@objdump -D -b binary -m i8086 $<

run: iso
	$(QEMU) -cdrom kern.iso

clean:
	@rm -f *.bin *.o *.log *.iso cdrom/boot/*.bin
