BUILDDIR?=.
OBJS=$(BUILDDIR)/plot.o $(BUILDDIR)/psf.o $(BUILDDIR)/kbd.o

.PHONY: clean

all: $(OBJS)

echo:
	echo $BUILDDIR

$(BUILDDIR)/%.o: %.c ../xdefs.h
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ -c $<

clean:
	-rm -f $(OBJS)
