# Configurations.
CFLAGS += -march=armv8-a -mtune=Cortex-A53 -Wall -ffreestanding -nostdlib -nostartfiles
GOALS += $(BOARD_SRC)/kernel8.img
BOARD_SRC = boards/rpi3
OUTPUT += $(BOARD_SRC)/kernel8.img $(BOARD_SRC)/kernel8.elf
OBJS += $(BOARD_SRC)/drivers.o \
	    $(BOARD_SRC)/gpu.o \
		$(BOARD_SRC)/mailbox.o \
		$(BOARD_SRC)/uart.o \
		$(BOARD_SRC)/rpi3.o


# Load the toolchain.
TOOLCHAIN ?= aarch64-elf-
AS = $(TOOLCHAIN)as
CC = $(TOOLCHAIN)gcc
LD = $(TOOLCHAIN)ld
OBJCOPY = $(TOOLCHAIN)objcopy
QEMU ?= qemu-system-aarch64