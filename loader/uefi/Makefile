CC=x86_64-w64-mingw32-gcc
CFLAGS=-ffreestanding -nostdlib -Wall -fpic  -Wl,--subsystem,10 -e EFI_MAIN

all: loader.efi

loader.efi:
	$(CC) $(CFLAGS) headers/*.c main.c -o loader.efi
	sudo mount /dev/sdd1 /mnt
	sudo cp loader.efi /mnt/EFI/BOOT/BOOTX64.EFI
	sudo umount /dev/sdd1

clean:
	rm loader.efi
