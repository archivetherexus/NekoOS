# NekoOS
A little microkernel + planned userland


> What is NekoOS?

Nothing special. Just a little project I like spending my free time on. Will probably not be as professional as Linux or any BSD.

> What is implemented?

Nothing desu. At it's current form it only boots and displays a message. Hopefully I'll be able to recreate my old kernel in a more organised way (^.^).

## File structure

- kernel/
	- The microkenrel.
- userland/
	- The userland.
	
## Compiling
1. Read and install all packages that are needed for compilation. This is specified currently in the kernel/requirements.md
2. Type `make` from the kernel directory.
3. Tesit it by running the generated nekoos.iso file that is located in the kernel/build/ directory with your prefered emulator.

## Plans for next push :3
* Threads.
* PS/2 Keyboard driver.
* PCI detecting.
* Enabling the A20 line.

## Goals
* Epic lulz and fun while making it.
* Create an operating system that is cute but *probably* useless in any practical scenario.
