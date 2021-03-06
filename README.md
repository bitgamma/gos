# Game Operating System

GOS is a [library operating system](https://en.wikipedia.org/wiki/Operating_system#Library) for x86 which can be used to build [unikernels](https://en.wikipedia.org/wiki/Unikernel). It is mostly meant to be used to create retro-ish games able to run on older hardware (486 and above). An example game of maxit using the library is part of the repository (you need to provide your own image/sound files though). It also includes some Python scripts to simplify bundling resources and creating a usable disk image.

GOS comes with its own bootloader and is not compatible with multiboot.

Most importantly, GOS is a hobby project realized durings weekends and evening. I am fully aware it is far from being feature complete and I have taken lots of shortcuts at both implementation and architectural level to see the system do something interesting as fast as possible. However simple games can be realized already.

Unikernels using GOS run in 32-bit protected mode. 64-bit mode is not supported yet.

## Features
* 32-bit protected mode
* 1ms resolution timers
* ability to run background tasks (is a sort of cooperative multitasking manner)
* PS/2 keyboard support (if Legacy USB support is enabled in BIOS, USB keyboards work too)
* PS/2 mouse support (if Legacy USB support is enabled in BIOS, USB mice might work too)
* PCI bus scanning
* IRQ support
* Serial ports
* Configurable memory layout
* OPL3-based sound support (Soundblaster, Adlib)
* Soundblaster 16 DSP
* AC'97 sound support (incomplete)
* Bootable from USB drives, HDD
* VBE2.0 mode detection/set mode (mode currently set by the bootloader)
* 2d library compatibile with any card with a linear framebuffer
* Animations
* Support for 256-colors bitmaps (currently VGA palette is assumed)
* Support for DRO sound files
* Support for PCM sound files
* A few utility functions/data structures
* Python scripts to bundle resource files (images, sounds) and make the image also bootable in emulators

## Notable missing features
* ATA driver: now the full image is loaded using BIOS INT13h by the bootloader
* Memory heap: meaning currently all memory is statically allocated
* Persistency: there is no filesystem and no way to persist data
* UEFI booting
* USB
* TCP/IP networking
* Graphic acceleration

This is not a full list, these are the huge ones that come to mind now. Honestly, I don't expect to ever implement the last 3 points (but who knows), however the other points will probably be implemented if the project continues. The ATA driver is the easy part, but I want to retain the ability to boot from USB which is easier done by loading everything when still in (un)real mode.

## Requirements
* 486 or compatible CPU (FPU not used)
* VBE2.0 compatible graphic card
* Soundblaster or compatible (if audio is desired)
* Some megs of RAM. The system requires a few kb, but since the app is fully loaded during boot, enough RAM is needed to accomodate all resources.

## Tested platforms
* [DOSBox-X](https://dosbox-x.com/)
* [86Box](https://github.com/86Box/86Box)
* [v86](https://github.com/copy/v86)
* [Bochs](https://bochs.sourceforge.io/)
* [QEMU](https://www.qemu.org/documentation/)
* a Samsung laptop from 2012

## Development
If you want to use GOS to develop your game, you are awesome! All you need is Python 3.x, an i686-elf gcc cross-compiler, nasm, make and GNU dd. If you are on a mac, install GNU dd using brew.

The best way is probably to fork the repository and add your game there. If you want to add features to the system, feel free to open a PR, they are very welcome!

As you can see in the example project, a yaml config file is used to configure kernel and bootloader parameters (like screen resolution, buffer sizes, etc)

Regarding the resource files, you can use PNG images, DRO (captures from DOSBox) sounds and WAV sounds. The images will be converted to 256-color bitmaps by resc.py if a palette is provided.

The 2d library does not yet support 24/32-bit color modes, but it should be an easy fix. Any resolution is supported out-of-the-box.

## License
The code is under [MPL2.0](LICENSE). In short, you are allowed to statically link GOS with your project (be it open or not). Changes made to GOS itself must be redistributed in source code form though. Read the full license for details.

The boot logo is under [CC BY 4.0](https://creativecommons.org/licenses/by/4.0/). You can use your own boot logo if you wish but it'd be nice if you kept the included one.