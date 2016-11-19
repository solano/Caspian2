all:
	@config/build.sh

clean:
	@config/clean.sh

iso: all
	@echo "Building ISO..."
	@config/iso.sh
	@echo "Done."

run: iso
	@echo "Emulating..."
	@qemu-system-i386 -cdrom Caspian2.iso 

runfast: all
	@echo "Emulating..."
	@qemu-system-i386 -kernel build/kernel/kernel.bin
