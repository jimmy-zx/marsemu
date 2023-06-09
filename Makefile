CFLAGS?=-std=gnu11 -Wall -Wpedantic -Wextra -g -DPLOT_DEBUG
export
BUILDDIR?=build
MIPSCC=mips-linux-gnu-gcc

.PHONY: client mipsclient server builddir utils

all: mipsclient server

builddir:
	-mkdir -p $(BUILDDIR)

client: builddir
	BUILDDIR=../$(BUILDDIR) make -C client

mipsclient: builddir
	BUILDDIR=../$(BUILDDIR) CC=$(MIPSCC) make -C client

utils: builddir
	BUILDDIR=../$(BUILDDIR) CC=$(MIPSCC) make -C utils

server: builddir
	BUILDDIR=../$(BUILDDIR) make -C server

examples: builddir mipsclient
	BUILDDIR=../$(BUILDDIR) CC=$(MIPSCC) make -C examples

clean:
	BUILDDIR=../$(BUILDDIR) make -C client clean
	BUILDDIR=../$(BUILDDIR) make -C server clean
	BUILDDIR=../$(BUILDDIR) make -C examples clean
