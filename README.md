# NekoOS
A little microkernel + planned userland

> Goal

A little kernel with a FAT32 fs that has a built-in LISP interpreter.

> What is NekoOS?

Nothing special. Just a little project I like spending my free time on. Will probably not be as professional as Linux or any BSD.

> What is implemented?

Nothing desu. At it's current form it only boots and displays a message. Hopefully I'll be able to recreate my old kernel in a more organised way (^.^).

## File structure

- system/
	- The kernel.c and other files.
- userland/
	- The userland.
	
## Compiling
Currently compiling is done by
`cd system; ./build.sh; cd ..`

Old Method:
1. Read and install all packages that are needed for compilation. This is specified currently in the kernel/requirements.md
2. Type `make` from the kernel directory.
3. Test it by running the generated nekoos.iso file that is located in the git root directory with your prefered emulator.

## Plans for next push :3
* FAT32
* Lisp interpreter

## Goals
* Epic lulz and fun while making it.
* Create an operating system that is cute but *probably* useless in any practical scenario.
* Mini FAT32
* LISP interpreter
