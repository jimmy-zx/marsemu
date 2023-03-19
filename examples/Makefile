BUILDDIR?=.

.PHONY: clean

$(BUILDDIR)/client_drawfont: client_drawfont.c ../build/psf.o ../build/plot.o

$(BUILDDIR)/%: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $^ -static

clean:
	-rm -f $(BUILDDIR)/client_drawfont
