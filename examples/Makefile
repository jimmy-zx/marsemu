BUILDDIR?=.

.PHONY: clean

all: $(BUILDDIR)/client_drawfont $(BUILDDIR)/client_drawbox

$(BUILDDIR)/client_drawfont: client/client_drawfont.c ../build/psf.o ../build/plot.o
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $^ -static

$(BUILDDIR)/client_drawbox: client/client_drawbox.s ../build/plot.o
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $^ -static

clean:
	-rm -f $(BUILDDIR)/client_drawfont
