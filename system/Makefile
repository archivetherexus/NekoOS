# This is the main build script.

# Set the target.
TARGET ?= rpi3

# Include the sub-configs.
include boards/$(TARGET)/$(TARGET)-config.mk

# Add the object files.
OBJS += main.o

# Include the sub-rules.
include boards/$(TARGET)/$(TARGET)-rules.mk

clean:
	rm -f $(OBJS) $(OUTPUT)

# Do the compilation!
all: $(GOALS)

