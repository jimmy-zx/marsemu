CFLAGS=-std=gnu11 -Wall -Wpedantic -Wextra -g

server: server.c
	$(CC) $(CFLAGS) -o server server.c -lX11 -lXext

clean:
	-rm -f server
