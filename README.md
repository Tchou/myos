#myos
A toy 'operating system' used for teaching purposes.

#List of files

- boot.s    : entry-point of the OS that implements the 'multiboot' signature and minimal initialisation.
- kernel.c  : high-level code of the OS
- link.ld   : linker script to generate a suitable binary
- Makefile  : build script


# Dependencies
To build the project, you need :
| Program           | Debian package |
| ----------------- | -------------- |
|  gcc              |    gcc         |
|  as               |    binutils    |
|  strip            |    binutils    |
|  qemu-system-i386 |    qemu        |


# Building

  ```make myos.bin```

builds the kernel


# Testing :

  ```qemu-system-i386 -kernel myos.bin```

(this requires qemu version 2.0.0 or higher).




# References

http://wiki.osdev.org/Tutorials

# Notes

This project does not use a cross compiler, but just compiles with
-m32 flag (to procduce x86 binary on a 64bit arch) and usr strip to
remove unwanted (debugging and info) sections from the final binary.
