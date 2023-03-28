BUILDDIR?=.

.PHONY: clean

all: $(BUILDDIR)/client_drawfont $(BUILDDIR)/client_drawcorner $(BUILDDIR)/client_whichkey $(BUILDDIR)/client_clearfb

$(BUILDDIR)/client_drawfont: client/client_drawfont.c ../build/psf.o ../build/plot.o
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $^ -static

$(BUILDDIR)/client_whichkey: client/client_whichkey.c ../build/kbd.o
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $^ -static

$(BUILDDIR)/client_clearfb: client/client_clearfb.c ../build/plot.o
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $^ -static

$(BUILDDIR)/client_drawcorner: client/client_drawcorner.s ../build/plot.o
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $^ -static

clean:
	-rm -f $(BUILDDIR)/client_drawfont
