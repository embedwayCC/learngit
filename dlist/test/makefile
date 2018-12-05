APP_NAME?=test

all:lib_make
	gcc -static -I../src main.c -L../src -ldlist -o ${APP_NAME}

lib_make:
	make -C ../src

clean:
	rm -rf ${APP_NAME}
