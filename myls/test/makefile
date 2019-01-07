APP_NAME?=test

all:lib_make
ifeq ($(var),static)
	gcc -static -g -I../src main.c -L../src -lls -o ${APP_NAME}
else
	gcc -g -I../src main.c -L../src -lls -o ${APP_NAME}
endif

lib_make:
	make -C ../src

clean:
	rm -rf ${APP_NAME}
