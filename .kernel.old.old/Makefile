# Simple kernel makefile by: tyrerexus

##### BEGIN CONFIG

ARCH=x86_64

# The full path of the directory that is used for puting build files in.
export BUILD=$(shell pwd)/build
export C_COMPILER=gcc
export C_FLAGS=-m32 -static -nostdlib -std=gnu99 -ffreestanding -O2 -Wall -Wextra
export LINKER=ld
export LINKER_FLAGS=-n -melf_i386
export SRCDIRS:= src arch/$(ARCH)/src
export SUBSRCDIRS+= arch/asm
#### END CONFIG

# Colors
export ccred=$(shell tput setaf 1)
export ccgreen=$(shell tput setaf 2)
export ccyellow=$(shell tput setaf 3)
export ccend=$(shell tput sgr0)

# All the .c files that are to be compiled.
SOURCES := $(foreach dir,$(SRCDIRS),$(shell find $(dir)/ -name "*.c"))
UNOBJECTS=$(SOURCES:.c=.o)
OBJECTS=$(addprefix $(BUILD)/, $(UNOBJECTS))

# Add the SRCDIRS to the include path.
C_FLAGS+=$(foreach dir,$(SRCDIRS),-I $(dir) )

# To make sure that the directory tree of all the src directroies exists in the build directory
SUBSRCDIRS+=$(foreach dir,$(SRCDIRS),$(shell find $(dir)/ -type d))
DIRECTORIES+=$(addprefix $(BUILD)/, $(SUBSRCDIRS))

# For the arch sub makefile
export SOURCES
export OBJECTS
export C_FLAGS

# Before kernel compilation we must make sure that the build directories exist.
all: $(BUILD) kernel
	@echo "$(ccgreen)Done!$(ccend)"

# Just print some help information. Nothing else.
help:
	@echo "Help for kernel build:"
	@echo "	make all (builds everything)"
	@echo "	make clean (cleans project)"
	@echo "	make run (runs the kernel via an emulator)"
	@echo ""

# Easy, just remove the build directory recursively.
clean:
	rm -r $(BUILD)

# The kernel is made out of two parts: 
# 1. The generic part that is platform independent.
# 2. And the platform dependent part known as arch.
kernel: generic-kernel arch-kernel
	@echo "$(ccgreen)DONE: Kernel compilation done...$(ccend)"

# This simply calls the current arch's makefile recursively
arch-kernel:
	@$(MAKE) kernel-arch -C arch/$(ARCH)
	@echo "$(ccgreen)DONE: Compiling arch specific kernel parts."
	@echo "$(ccend)"

# Here we compile the generic kernel by compiling the .o files for the generic part.
generic-kernel: $(OBJECTS)
	@echo "$(ccgreen)DONE: Compiling generic kernel parts."
	@echo "$(ccend)"

# Automatic dependency graph generation
-include $(OBJECTS:.o=.d)

# Here we compile a object file using it's c file partner.
$(BUILD)/%.o: %.c
	$(C_COMPILER) $(C_FLAGS) -c -MMD -MT $@ -MF $(patsubst %.o,%.d,$@) $< -o $@


# Create some directories
$(BUILD):
	@echo "$(ccyellow)Creating directories:"
	@$(foreach dir,$(DIRECTORIES),mkdir -p $(dir);echo $(dir);)
	@echo "$(ccend)"
