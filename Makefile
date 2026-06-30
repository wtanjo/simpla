CC ?= gcc
CFLAGS ?= -Wall -Wextra -ffast-math -fopenmp -O3 -march=native
LIBS ?=
DFLAGS ?= -fPIC -shared
SFLAGS ?= -c
EGLIBS ?= -I./include -L./lib -lsimpla

PREFIX ?= /usr/local

all: lib include

lib: simpla.c
	mkdir -p lib
	$(CC) $(CFLAGS) $(DFLAGS) -o lib/libsimpla.so simpla.c $(LIBS)
	$(CC) $(CFLAGS) $(SFLAGS) -o lib/simpla.o simpla.c $(LIBS)
	ar rcs lib/libsimpla.a lib/simpla.o

include: simpla.h
	mkdir -p include
	cp simpla.h include/

example: example.c lib include
	$(CC) $(CFLAGS) -o example example.c $(EGLIBS)

runeg: example
	export LD_LIBRARY_PATH=./lib:$LD_LIBRARY_PATH && ./example

clean:
	@if [ -d lib ]; then rm -rf lib; fi
	@if [ -d include ]; then rm -rf include; fi
	@if [ -f example ]; then rm example; fi

install: lib include
	install -d $(PREFIX)/include/simpla/
	install -Dm644 lib/* $(PREFIX)/lib/
	install -Dm644 include/* $(PREFIX)/include/simpla/

uninstall:
	rm $(PREFIX)/lib/libsimpla.a $(PREFIX)/lib/libsimpla.so $(PREFIX)/lib/simpla.o
	rm -rf $(PREFIX)/include/simpla

.PHONY: all clean runeg release install uninstall
