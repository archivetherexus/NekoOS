$(BOARD_SRC)/kernel8.img: $(BOARD_SRC)/kernel8.elf
	$(OBJCOPY) -O binary $< $@

$(BOARD_SRC)/kernel8.elf: $(BOARD_SRC)/rpi3-linker.ld $(OBJS)
	$(LD) -nostdlib -nostartfiles $(OBJS) -T $(BOARD_SRC)/rpi3-linker.ld -o $@

%.o: %.s
	$(AS) -c $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

qemu: $(BOARD_SRC)/kernel8.img
	$(QEMU) -M raspi3 -kernel boards/rpi3/kernel8.img -serial stdio