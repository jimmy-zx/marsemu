BUILDDIR?=.

$(BUILDDIR)/server: server.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $^ -lX11 -lXext

clean:
	-rm -f $(BUILDDIR)/server
