CFLAGS=-Wall -Wextra -Wpedantic -std=gnu11 -g

client_drawfont: client_drawfont.c ../client/plot.o ../client/psf.o
	mips-linux-gnu-gcc -static -o $@ $^

%: %.c
	$(CC) $(CFLAGS) -o $@ $^ -lX11 -lXext

TARGET=shm_putimage xlib_shmimage

all: $(TARGET)

client_drawline: client_drawline.c ../client/plot.o


.PHONY: clean

clean:
	-rm -f $(TARGET)
