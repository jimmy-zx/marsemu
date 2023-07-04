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
This [folder](/examples/asm) provides some examples and a macro to help you setup the stack frame.

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

## Notes

- `ssh -X` will not be supported (see [here](https://unix.stackexchange.com/questions/534314/working-with-the-mit-shm-x11-extension-on-linux))

- Users using WSL: use WSLg instead

- Scaling is not natively supported. You can use [run\_scaled](https://github.com/kaueraal/run_scaled) on X11 or `magnify.exe` on Windows.

### Porting to MARS

- MARS does not support several derivatives used by GNU as.
A simple [preprocessor](/utils/asmpp.py) is provided to assess some of these.

## Related Resources

### Xlib
[One tutorial](https://handmade.network/forums/articles/t/2834-tutorial_a_tour_through_xlib_and_related_technologies),
[Another tutorial](https://tronche.com/gui/x/xlib-tutorial/)

### Xlib MIT-SHM Extension
[XShmXXXImage() call](https://stackoverflow.com/questions/43442675/how-to-use-xshmgetimage-and-xshmputimage),
[Design choice](https://stackoverflow.com/questions/34176795/any-efficient-way-of-converting-ximage-data-to-pixel-map-e-g-array-of-rgb-quad)

### SHM
[shmat() system call](https://www.csl.mtu.edu/cs4411.ck/www/NOTES/process/shm/shmat.html)

### PSF
[PC Screen Font Specification](https://www.win.tue.nl/~aeb/linux/kbd/font-formats-1.html),
[Guide from osdev.org](https://wiki.osdev.org/PC_Screen_Font)
