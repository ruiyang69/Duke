CC=gcc
CFLAGS=-O3 -fPIC
DEPS=my_malloc.h

all: lib

lib: my_malloc.o
	$(CC) $(CFLAGS) -g -shared -o libmymalloc.so my_malloc.o

%.o: %.c my_malloc.h
	$(CC) $(CFLAGS) -c -g -o $@ $< 

clean:
	rm -f *~ *.o *.so

clobber:
	rm -f *~ *.o
