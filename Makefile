#CC ?= 
CFLAGS ?= -Wall -Wextra -ffast-math -fopenmp
DFLAGS ?= -fPIC -shared
SFLAGS ?= -c
RFLAGS ?= -O3
EGFLAGS ?= -I./include -L./lib -lsimpla

PREFIX ?= /usr/local

all: lib include

lib: simpla.c
	mkdir -p lib
	$(CC) $(CFLAGS) $(DFLAGS) -o lib/libsimpla.so simpla.c
	$(CC) $(CFLAGS) $(SFLAGS) -o lib/simpla.o simpla.c
	ar rcs lib/libsimpla.a lib/simpla.o

include: simpla.h
	mkdir -p include
	cp simpla.h include/

example: example.c lib include
	$(CC) $(CFLAGS) -o example example.c $(EGFLAGS)

runeg: example
	export LD_LIBRARY_PATH=./lib:$LD_LIBRARY_PATH && ./example

clean:
	@if [ -d lib ]; then rm -rf lib; fi
	@if [ -d include ]; then rm -rf include; fi
	@if [ -f example ]; then rm example; fi

release: simpla.c simpla.h
	mkdir -p lib
	$(CC) $(CFLAGS) $(DFLAGS) -o lib/libsimpla.so simpla.c $(RFLAGS)
	$(CC) $(CFLAGS) $(SFLAGS) -o lib/simpla.o simpla.c $(RFLAGS)
	ar rcs lib/libsimpla.a lib/simpla.o
	mkdir -p include
	cp simpla.h include/


install: lib include
	install -d $(PREFIX)/include/simpla/
	install -Dm644 lib/* $(PREFIX)/lib/
	install -Dm644 include/* $(PREFIX)/include/simpla/

uninstall:
	rm $(PREFIX)/lib/libsimpla.a $(PREFIX)/lib/libsimpla.so $(PREFIX)/lib/simpla.o
	rm -rf $(PREFIX)/include/simpla

.PHONY: all clean runeg release install uninstall
