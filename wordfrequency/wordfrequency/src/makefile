LIB_NAME?=wordfrequency

all:static_library shared_library

static_library:lib_dlist_make
	gcc -g -c *.c
	ar -cr lib${LIB_NAME}.a *.o

shared_library:lib_dlist_make
	gcc -g -shared -fpic -o lib${LIB_NAME}.so *.c

lib_dlist_make:
	make -C ../../dlist/src

clean:
	rm *.o
	rm *.a *.so