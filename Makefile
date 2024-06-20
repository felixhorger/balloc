.PHONY: lib, test

build/bmalloc.o: src/bmalloc.c
	gcc -g -fPIC -c src/bmalloc.c -o build/bmalloc.o
#

lib/libbmalloc.so: build/bmalloc.o
	gcc -g -fPIC -shared -o $@ build/bmalloc.o
#

lib/libbmalloc.a: build/bmalloc.o
	ar rcs $@ build/bmalloc.o
#

lib: lib/libbmalloc.a lib/libbmalloc.so

test/test: test/test.c lib/libbmalloc.a Makefile
	gcc -o test/test -g test/test.c lib/libbmalloc.a
#
#gcc -g -c test/test.c -o build/test.o see https://stackoverflow.com/questions/480764/linux-error-while-loading-shared-libraries-cannot-open-shared-object-file-no-s

test: test/test

