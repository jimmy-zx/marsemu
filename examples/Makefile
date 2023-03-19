%: %.c
	$(CC) $(CFLAGS) -o $@ $^ -lX11 -lXext

TARGET=shm_putimage xlib_shmimage

all: $(TARGET)

.PHONY: clean

clean:
	-rm -f $(TARGET)
