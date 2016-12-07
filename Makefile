all:
	@config/build.sh

clean:
	@config/clean.sh

iso: all
	@echo "Building ISO..."
	@config/iso.sh
	@echo "Done."

runiso: iso
	@echo "Emulating..."
	@qemu-system-i386 -cdrom Caspian2.iso 

run: all
	@echo "Emulating..."
	@./run.sh

debug: all
	@echo "Emulating..."
	@./debug.sh
