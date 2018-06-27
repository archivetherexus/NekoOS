# This is the main build script.

CLANG_FORMAT ?= clang-format

# Set the target.
TARGET ?= rpi3

# Include the sub-configs.
include boards/$(TARGET)/$(TARGET)-config.mk

# Add the object files.
OBJS += main.o kfont/draw.o kfont/default.o kdraw/shapes.o
CFLAGS += -I.

# Include the sub-rules.
include boards/$(TARGET)/$(TARGET)-rules.mk

clean:
	rm -f $(OBJS) $(OUTPUT)
	$(CLANG_FORMAT) -style=file -i $(shell find . -type f -name '*.c')

# Do the compilation!
all: $(GOALS)

