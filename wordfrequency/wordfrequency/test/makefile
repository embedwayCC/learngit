APP_NAME?=test1

all:lib_wordfrequency_make
ifeq ($(var),static)
	gcc -static -g -I../src -I../../dlist/src main.c -L../src -lwordfrequency -L../../dlist/src -ldlist -o ${APP_NAME}
else
	gcc -g -I../src -I../../dlist/src main.c -L../src -lwordfrequency -L../../dlist/src -ldlist -o ${APP_NAME}
endif

lib_wordfrequency_make:
	make -C ../src

clean:
	rm -rf ${APP_NAME}

