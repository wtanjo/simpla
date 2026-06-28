#CC = 
CFLAGS = -Wall -Wextra
CLIBFLAGS = -fPIC -shared
LIBS = -L. -lsimpla

all: example

libsimpla.so: simpla.c
	$(CC) $(CLIBFLAGS) -o libsimpla.so simpla.c

example: example.c simpla.h libsimpla.so
	$(CC) $(CFLAGS) -o example example.c $(LIBS)

runeg: example
	export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH && ./example

clean:
	@if [ -f libsimpla.so ]; then rm libsimpla.so; fi
	@if [ -f example ]; then rm example; fi

.PHONY: all clean runeg
