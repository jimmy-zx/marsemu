BUILDDIR?=.

$(BUILDDIR)/printpsf: printpsf.c ../build/psf.o ../build/plot.o
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $^ -static
