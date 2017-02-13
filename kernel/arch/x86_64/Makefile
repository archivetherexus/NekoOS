# Add all the objects made by the arch.

ARCH_SOURCES+= $(shell find ./asm -name "*.asm")
ARCH_UNOBJECTS=$(ARCH_SOURCES:.asm=.o)
ARCH_OBJECTS=$(addprefix $(BUILD)/arch/, $(ARCH_UNOBJECTS))

wrong-makefile:
	@echo "ERROR: Wrong makefile."
	@echo "For the correct one do:"
	@echo "	make -C ../.."

kernel-arch: $(BUILD)/kernel.bin iso-file

$(BUILD)/kernel.bin: $(ARCH_OBJECTS)
	$(LINKER) $(LINKER_FLAGS) -T linker.ld -o $(BUILD)/kernel.bin $(OBJECTS) $(ARCH_OBJECTS)
	@echo "$(ccgreen)DONE: Making kernel for arch: x86_64"
	@echo "$(ccend)"

#as --32 bootstrap.s -o $(BUILD)/arch/bootstrap.o
$(BUILD)/arch/%.o: %.asm
	nasm -f elf32 -o $@ $<

iso-file: $(BUILD)/nekoos.iso

$(BUILD)/nekoos.iso: $(BUILD)/kernel.bin
	@echo "Making .iso file."
	mkdir -p $(BUILD)/isodir/boot/grub
	cp $(BUILD)/kernel.bin $(BUILD)/isodir/boot/kernel.bin
	cp grub.cfg $(BUILD)/isodir/boot/grub/grub.cfg
	grub-mkrescue --xorriso=xorriso -o $(BUILD)/nekoos.iso $(BUILD)/isodir
	@echo "$(ccgreen)DONE: Making .iso file$(ccend)"
	@echo ""
