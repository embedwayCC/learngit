LIB_NAME?=dlist

all:static_library shared_library

static_library:
	gcc -c *.c
	ar -cr lib${LIB_NAME}.a *.o

shared_library:
	gcc -shared -fpic -o lib${LIB_NAME}.so *.c

clean:
	rm *.o
	rm *.a *.so
