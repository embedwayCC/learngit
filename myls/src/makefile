LIB_NAME?=ls

all:static_library shared_library

static_library:
	gcc -g -c *.c
	ar -cr lib${LIB_NAME}.a *.o

shared_library:
	gcc -g -shared -fpic -o lib${LIB_NAME}.so *.c

clean:
	rm *.o
	rm *.a *.so

