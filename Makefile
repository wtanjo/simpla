CC = gcc
CFLAGS ?= -Wall -Wextra -ffast-math -fopenmp -O3 -march=native
LIBS ?=
DFLAGS ?= -fPIC -shared
SFLAGS ?= -c
EGLIBS ?= -I./include -L./lib -lsimpla

PREFIX ?= /usr/local

all: lib include man

lib: simpla.c man.h
	mkdir -p lib
	$(CC) $(CFLAGS) $(DFLAGS) -o lib/libsimpla.so simpla.c $(LIBS)
	$(CC) $(CFLAGS) $(SFLAGS) -o lib/simpla.o simpla.c $(LIBS)
	ar rcs lib/libsimpla.a lib/simpla.o

include: simpla.h
	mkdir -p include
	cp simpla.h include/

example: example.c
	$(CC) -o example example.c $(EGLIBS)

runeg: example
	export LD_LIBRARY_PATH=./lib:${LD_LIBRARY_PATH} && ./example

clean:
	@if [ -d lib ]; then rm -rf lib; fi
	@if [ -d include ]; then rm -rf include; fi
	@if [ -f example ]; then rm example; fi
	@if [ -d man3 ]; then rm -rf man3; fi

install: lib include man
	install -d $(PREFIX)/include/simpla/
	install -Dm644 lib/simpla.o lib/libsimpla.a lib/libsimpla.so $(PREFIX)/lib/
	install -Dm644 include/simpla.h $(PREFIX)/include/simpla/
	install -d $(PREFIX)/share/man/man3/
	install -D man3/simpla.3 $(PREFIX)/share/man/man3/

uninstall:
	rm $(PREFIX)/lib/libsimpla.a $(PREFIX)/lib/libsimpla.so $(PREFIX)/lib/simpla.o
	rm -rf $(PREFIX)/include/simpla
	rm -rf $(PREFIX)/share/man/man3/simpla.3

man: simpla.3.scd
	mkdir -p man3
	scdoc < simpla.3.scd > man3/simpla.3

.PHONY: all clean runeg release install uninstall man
