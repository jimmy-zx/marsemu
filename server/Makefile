BUILDDIR?=.

$(BUILDDIR)/server: server.c ../xdefs.h
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $< -lX11 -lXext

clean:
	-rm -f $(BUILDDIR)/server
