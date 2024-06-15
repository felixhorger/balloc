#include <stdlib.h>
#include <stdio.h>
#include "../include/bmalloc.h"

void test_lmalloc()
{
	size_t n = 30;
	int *a = (int *)lmalloc(n * sizeof(int), 100);
	for (int i = 0; i < n; i++) lset(a, i, 0);

	lset(a, 9, 1);
	lset(a, 10, 1);
	lset(a, 11, 1);
	lset(a, 29, 1);

	size_t m = 60;
	a = lrealloc(a, m * sizeof(int));

	size_t *s = (size_t *)a;
	// last accessed block
	void *lastblock = (void *)s[0]; // Not 100% clean, because requires sizeof(size_t) == sizeof(void **), is this guaranteed?
	size_t lastblock_idx = s[1];
	size_t blocksize = s[2];

	//printf("%p %ld %ld \n", lastblock, lastblock_idx, blocksize);

	printf("%p\n",                                             s[3]                           );
	printf("%p\n",                          *(void **)((void*)(s[3]) + 100)                   );
	printf("%p\n",             *((void **)((*(void **)((void*)(s[3]) + 100)) + 100))          );
	printf("%p\n", *((void **)(*((void **)((*(void **)((void*)(s[3]) + 100)) + 100)) + 100))  );

	for (int i = 0; i < m; i++) lset(a, i, i);
	//for (int i = 0; i < m; i++) printf("%d:\t%d\n", i, lget(a, i));

	m = 30;
	a = lrealloc(a, m * sizeof(int));
	printf("%p\n",                                             s[3]                           );
	printf("%p\n",                          *(void **)((void*)(s[3]) + 100)                   );

	for (int i = 0; i < m; i++) lset(a, i, 2*i);
	for (int i = 0; i < m; i++) printf("%d:\t%d\n", i, lget(a, i));


	lfree(a);

	return;
}

int main(int argc, char **argv)
{
	test_lmalloc();
	return 0;
	

	size_t n = 10;
	int *a = (int *)bmalloc(n * sizeof(int), 100);
	for (int i = 0; i < n; i++) bset(a, i, 0);


	bset(a, 9, 1);
	bset(a, 10, 1);
	bset(a, 11, 1);

	size_t m = 500;
	a = brealloc(a, m * sizeof(int));

	bset(a, 499, 1);


	for (int i = 0; i < m; i++) printf("%d:\t%d\n", i, bget(a, i));
	//printf("%d\n", *((size_t*) a));

	a = brealloc(a, 10 * sizeof(int));

	for (int i = 0; i < 10; i++) printf("%d:\t%d\n", i, bget(a, i));

	bfree(a);

	return 0;
}

