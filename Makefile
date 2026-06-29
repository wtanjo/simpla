#CC ?= 
CFLAGS ?= -Wall -Wextra
DFLAGS ?= -fPIC -shared
SFLAGS ?= -c
RFLAGS ?= -O3
EGLIBS ?= -L./lib -lsimpla

PREFIX ?= /usr/local

all: lib include

lib: libsimpla.so simpla.o libsimpla.a
	mkdir -p lib
	mv libsimpla.so simpla.o libsimpla.a lib

include: simpla.h
	mkdir -p include
	cp simpla.h include/

libsimpla.so: simpla.c
	$(CC) $(CFLAGS) $(DFLAGS) -o libsimpla.so simpla.c

simpla.o: simpla.c
	$(CC) $(CFLAGS) $(SFLAGS) -o simpla.o simpla.c

libsimpla.a: simpla.o
	ar rcs libsimpla.a simpla.o

example: example.c simpla.h all
	$(CC) $(CFLAGS) -o example example.c $(EGLIBS)

runeg: example
	export LD_LIBRARY_PATH=./lib:$LD_LIBRARY_PATH && ./example

clean:
	@if [ -d lib ]; then rm -rf lib; fi
	@if [ -d include ]; then rm -rf include; fi
	@if [ -f example ]; then rm example; fi

release: simpla.c simpla.h
	$(CC) $(CFLAGS) $(DFLAGS) -o libsimpla.so simpla.c $(RFLAGS)
	$(CC) $(CFLAGS) $(SFLAGS) -o libsimpla.o simpla.c $(RFLAGS)
	ar rcs libsimpla.a simpla.o
	$(MAKE) lib
	$(MAKE) include

install: lib include
	install -Dm644 lib/* $(PREFIX)/lib/
	install -Dm644 include/* $(PREFIX)/include/simpla/

uninstall:
	rm $(PREFIX)/lib/libsimpla.a $(PREFIX)/lib/libsimpla.so $(PREFIX)/lib/simpla.o
	rm -rf $(PREFIX)/include/simpla

.PHONY: all clean runeg release install uninstall
