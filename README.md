# MARSEMU

X11 virtual framebuffer with keyboard input for cross-debugging.

A project to emulate [MARS](https://courses.missouristate.edu/kenvollmar/mars/)'s bitmap display and keyboard.

Tested under x86 and qemu-mips.

## Quick start

```bash
git clone https://github.com/jimmy-zx/marsemu.git
cd marsemu
make
make examples
./build/server &  # or run in another terminal
./build/client_drawcorner
```

## Dependencies

- `gcc`
- `gcc-mips-linux-gnu`
- `libx11-dev`
- `libxext-dev`

All of the dependencies can be installed via `apt`:
```bash
sudo apt install build-essential gcc-mips-linux-gnu libx11-dev libxext-dev
```

## System Requirements

Host: `x86`

A X11 server with MIT-SHM support. (The server and client must run on the same host.)

Emulation:
- `qemu-system-mips`
- `qemu-user`

Debugging
- `gdb-multiarch`

### Notes

- `ssh -X` won't be supported (see [here](https://unix.stackexchange.com/questions/534314/working-with-the-mit-shm-x11-extension-on-linux))

- Users using WSL: use WSLg instead

## Features

### Server
- Virtual Framebuffer
- Keyboard

### Client
- [plot](/client/plot.h): draw pixels directly
- [psf](/client/psf.h): draw letters using PSF fonts
- [kbd](/client/kbd.h): get keyboard input

## Usage

Example usage using MIPS asm: [client\_drawcorner.s](/examples/client/client_drawcorner.s).
Compilation instructions: [Makefile](/examples/Makefile)

Calling conventions: follow [System V MIPS ABI](https://refspecs.linuxfoundation.org/elf/mipsabi.pdf).
This [folder](/examples/asm) provides some examples and a macro.

## Architecture

```
+----------+     +----------------+         +----------------------------+
| X Server | <-> | MARSEMU Server | <-SHM-> | QEMU-user                  |
+----------+     +----------------+         | +------------------------+ |
                                            | | MARSEMU Client Library | |
                                            | +------------------------+ |
                                            |            ||              |
                                            | +------------------------+ |
                                            | | Userspace MIPS Program | |
                                            | +------------------------+ |
                                            +----------------------------+

```

## References:

- Xlib
[1](https://handmade.network/forums/articles/t/2834-tutorial_a_tour_through_xlib_and_related_technologies)
[2](https://tronche.com/gui/x/xlib-tutorial/)

- Xlib MIT-SHM Extension
[1](https://stackoverflow.com/questions/43442675/how-to-use-xshmgetimage-and-xshmputimage)
[2](https://stackoverflow.com/questions/34176795/any-efficient-way-of-converting-ximage-data-to-pixel-map-e-g-array-of-rgb-quad)

- SHM
[1](https://www.csl.mtu.edu/cs4411.ck/www/NOTES/process/shm/shmat.html)

- PSF
[1](https://www.win.tue.nl/~aeb/linux/kbd/font-formats-1.html)
[2](https://wiki.osdev.org/PC_Screen_Font)
