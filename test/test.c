#include <stdlib.h>
#include <stdio.h>
#include "../include/bmalloc.h"

int main(int argc, char **argv)
{
	
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

