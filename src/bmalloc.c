
#include <stdlib.h>
#include <stdio.h>

void * bmalloc(size_t size, size_t blocksize)
{
	size_t n = size / blocksize + (size % blocksize != 0);
	void *ptr = malloc(2 * sizeof(size_t) + n * sizeof(void *));

	size_t *s = (size_t *)ptr;
	*s = n;
	*(s+1) = blocksize;

	void **blocks = (void **)s+2;
	for (void **b = blocks; b < blocks+n; b++) *b = malloc(blocksize);

	return ptr;
}


extern inline void* bptr(void *ptr, size_t stride, size_t i)
{
	size_t *s = (size_t *)ptr;
	size_t blocksize = *(s+1);
	char **blocks = (char **)(s+2);

	i *= stride;
	return blocks[i / blocksize] + (i % blocksize);
}

#define BGET(TYPE, PTR, IDX)      ( *((TYPE *)bptr(PTR, sizeof(TYPE), IDX)) )
#define BSET(TYPE, PTR, IDX, VAL) ( *((TYPE *)bptr(PTR, sizeof(TYPE), IDX)) = VAL )

void * brealloc(void *ptr, size_t size)
{
	size_t *s = (size_t *)ptr;
	size_t n = *s;
	size_t blocksize = *(s+1);
	size_t oldsize = n * blocksize;

	if (oldsize == size) return ptr;

	size_t m = size / blocksize + (size % blocksize == 0);
	*s = m;
	void **blocks = (void **)(s+2);

	if (oldsize > size) for (void **b = blocks+m; b < blocks+n; b++) free(*b);

	ptr = realloc(ptr, 2 * sizeof(size_t) + m * sizeof(void *));

	if (oldsize < size) for (void **b = blocks+n; b < blocks+m; b++) *b = malloc(blocksize);

	return ptr;
}


void bfree(void *ptr)
{
	size_t *s = (size_t *)ptr;
	size_t n = *s;
	void **blocks = (void **)(s+2);
	for (void **b = blocks; b < blocks+n; b++) free(*b);
	free(ptr);
	return;
}



int main(int argc, char **argv)
{
	
	size_t n = 1000;
	int *a = (int *)bmalloc(n * sizeof(int), 100);

	for (int i = 0; i < n; i++) BSET(int, a, i, 0);

	BSET(int, a, 10, 1);
	BSET(int, a, 11, 1);
	BSET(int, a, 999, 1);

	size_t m = 500;
	a = brealloc(a, m * sizeof(int));

	for (int i = 0; i < m; i++) printf("%d:\t%d\n", i, BGET(int, a, i));

	bfree(a);

	return 0;
}

