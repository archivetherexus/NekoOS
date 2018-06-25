---
title: Information about hte 
---

# Abstract
This document contains information about compiling and runnning NekoOS 
on the RPi 3.

# Installing the Toolchain
You will want to download the toolchain from this [site](https://releases.linaro.org/components/toolchain/binaries/latest/aarch64-elf/).  
And then extract and add the `bin` directory to your path.
Something like this will do in both ZSH and Bash:
`export PATH=$HOME/.local/gcc-linaro-7.2.1-2017.11-x86_64_aarch64-elf/bin:$PATH`
when the toolchain is extracted into the .local directory.

# Files
* `rpi3.mk` The build script.
* `rpi3.s` Contains the startup code and some helper procedures.
* `rpi3`

# Information
These are some great resources for understanding baremetal programming on the RPi3:
* https://github.com/bztsrc/raspi3-tutorial
* https://wiki.osdev.org/Raspberry_Pi_3
* https://wiki.osdev.org/Raspberry_Pi_Bare_Bones

# Installing QEMU
You will probably want to install a custom built QEMU to your system since 
most distrobutions don't ship with the newest aarch64-softmmmu version of QEMU.  
To do so first download the QEMU source files from a 
[mirror on GitHub](https://github.com/qemu/qemu).
Then cd into the directory and run `./configure --target-list="aarch64-softmmu"`.  
This will configure the build scripts so that only the aarch64 version is built, which is what we want for RPi3. Why do so? Because it is faster that way.  
Then run `make` as you would usually do.
When this is done, add `aarch64-softmmu/qemu-system-aarch64` to the path by either adding `aarch64-softmmu` to the PATH variable, or by symlinking qemu-system-aarch64 from `.local/bin` or some other directory that is already in your PATH.

OBS: You might need to install some dependencies for this, which is not covered here.