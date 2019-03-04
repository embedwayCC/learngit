APP_NAME1?=test_server
APP_NAME2?=test_client
LIB_NAME?=wrap

all:lib_make
ifeq ($(var),static)
	gcc -static -g -I./ server.c -L./ -l${LIB_NAME} -o ${APP_NAME1}
	gcc -static -g -I./ client.c -L./ -l${LIB_NAME} -o ${APP_NAME2}
else
	gcc -g -I./ server.c -L./ -l${LIB_NAME} -o ${APP_NAME1}
	gcc -g -I./ client.c -L./ -l${LIB_NAME} -o ${APP_NAME2}
endif

lib_make:static_library shared_library

static_library:
	gcc -g -c wrap.c
	ar -cr lib${LIB_NAME}.a wrap.o

shared_library:
	gcc -g -shared -fpic -o lib${LIB_NAME}.so wrap.c

clean:
	rm *.o
	rm *.a *.so
	rm -rf ${APP_NAME1}
	rm -rf ${APP_NAME2}
