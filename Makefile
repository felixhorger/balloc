.PHONY: lib, test

build/bmalloc.o: src/bmalloc.c
	gcc -fPIC -c src/bmalloc.c -o build/bmalloc.o
#

lib/libbmalloc.so: build/bmalloc.o
	gcc -fPIC -shared -o $@ build/bmalloc.o
#

lib/libbmalloc.a: build/bmalloc.o
	ar rcs $@ build/bmalloc.o
#

lib: lib/libbmalloc.a lib/libbmalloc.so

test/test: test/test.c lib/libbmalloc.a
	gcc -g test/test.c lib/libbmalloc.a -o test/test
#

test: test/test

