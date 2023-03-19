BUILDDIR?=.
OBJS=$(BUILDDIR)/plot.o $(BUILDDIR)/psf.o

.PHONY: clean

all: $(OBJS)

echo:
	echo $BUILDDIR

$(BUILDDIR)/%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ -c $^

clean:
	-rm -f $(OBJS)
