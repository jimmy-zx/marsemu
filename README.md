# MARSEMU

A project to emulate MARS's bitmap display and keyboard.

Tested under x86 and qemu-mips.

## Quick start

```bash
git clone https://github.com/jimmy-zx/marsemu.git
cd marsemu
make
make examples
./build/server &
./build/client_drawfont
```

## Features

### Server
- Virtual Framebuffer
- Keyboard

### Client
- [plot](/client/plot.h): draw pixels directly
- [psf](/client/psf.h): draw letters using PSF fonts
- [kbd](/client/kbd.h): get keyboard input

## Architecture

```
+----------+     +----------------+         +----------------------------+
| X Server | <-> | MARSEMU Server | <-SHM-> | QEMU-user                  |
+----------+     +----------------+         | +------------------------+ |
                                            | | MARSEMU Client Library | |
                                            | +------------------------+ |
                                            |            ||              |
                                            | +------------------------+ |
                                            | | MIPS Program           | |
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
