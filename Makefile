CC:=gcc
AS:=as
LD:=ld
QEMU:=qemu-system-i386

QEMUOPTS:=

KERN_LINK_SCRIPT:=kernel.ld

KERN_CFLAGS:= -ffreestanding -nostdlib -m32
KERN_LDFLAGS:= -T $(KERN_LINK_SCRIPT) -m elf_i386

bootloader.bin: bootloader.o
	$(LD) -m elf_i386 -Ttext 0x7c00 --oformat binary -o $@ $<

# lowkern.o: lowkern.S
# 	$(CC) $(KERN_CFLAGS) -o $@ -c $<

# lowkern.bin: lowkern.o
# 	$(LD) $(KERN_LDFLAGS) -o $@ $<

boot.o: boot.S
	$(CC) $(KERN_CFLAGS) -o $@ -c $<

kernel.o: kernel.c
	$(CC) $(KERN_CFLAGS) -o $@ -c $<

ckern.bin: boot.o kernel.o
	$(LD) $(KERN_LDFLAGS) -o $@ kernel.o boot.o

iso: ckern.bin
	@cp ckern.bin cdrom/boot/
	@grub-mkrescue -o kern.iso cdrom

bootloader.o: bootloader.S
	$(AS) --32 -o $@ $<

dump_bl: bootloader.bin
	@objdump -D -b binary -m i8086 $<

run-bl: bootloader.bin
	$(QEMU) -nographic -drive file=bootloader.bin,index=0,media=disk,format=raw $(QEMUOPTS)

clean:
	@rm -f *.bin *.o *.log *.iso
