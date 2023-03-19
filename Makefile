CFLAGS?=-std=gnu11 -Wall -Wpedantic -Wextra -g
export
BUILDDIR?=build
MIPSCC=mips-linux-gnu-gcc

.PHONY: client mipsclient server builddir

all: mipsclient server

builddir:
	-mkdir -p $(BUILDDIR)
	-mkdir -p $(BUILDDIR)/mips

client: builddir
	BUILDDIR=../$(BUILDDIR) make -C client

mipsclient: builddir
	BUILDDIR=../$(BUILDDIR)/mips CC=$(MIPSCC) make -C client

server: builddir
	BUILDDIR=../$(BUILDDIR) make -C server

clean:
	BUILDDIR=../$(BUILDDIR) make -C client clean
