
#include <stdlib.h>
#include <stdio.h>
#include "../include/bmalloc.h"

void *bmalloc(size_t size, size_t blocksize)
{
	// TODO: what happens if size == 0?
	size_t n = size / blocksize + (size % blocksize != 0);
	void *ptr = malloc(2 * sizeof(size_t) + n * sizeof(void *));

	size_t *s = (size_t *)ptr;
	*s = n;
	*(s+1) = blocksize;

	void **blocks = (void **)(ptr)+2;
	for (void **b = blocks; b < blocks+n; b++) *b = malloc(blocksize);

	return ptr;
}

void *brealloc(void *ptr, size_t size)
{
	size_t *s = (size_t *)ptr;
	size_t n = *s;
	size_t blocksize = *(s+1);
	size_t oldsize = n * blocksize;

	if (oldsize == size) return ptr;

	size_t m = size / blocksize + (size % blocksize != 0);
	if (m == n) return ptr;

	*s = m;
	void **blocks = (void **)(ptr)+2;

	if (oldsize > size) for (void **b = blocks+m; b < blocks+n; b++) free(*b);

	ptr = realloc(ptr, 2 * sizeof(size_t) + m * sizeof(void *));
	blocks = (void **)(ptr)+2;

	if (oldsize < size) for (void **b = blocks+n; b < blocks+m; b++) *b = malloc(blocksize);

	return ptr;
}

void *bptr(void *ptr, size_t stride, size_t i)
{
	size_t *s = (size_t *)ptr;
	size_t blocksize = *(s+1);
	char **blocks = (char **)(s+2);

	i *= stride;
	return blocks[i / blocksize] + (i % blocksize);
}


void bfree(void *ptr)
{
	size_t *s = (size_t *)ptr;
	size_t n = *s;
	void **blocks = (void **)(ptr)+2;
	for (void **b = blocks; b < blocks+n; b++) free(*b);
	free(ptr);
	return;
}



// List like malloc

void *lmalloc(size_t size, size_t blocksize)
{
	size_t len = blocksize + sizeof(void *);
	size_t headsize = sizeof(void *) + sizeof(size_t) + sizeof(size_t) + sizeof(void **);
	// ptr to last block accessed, index of last block accessed, blocksize, pointer to block
	size_t *ptr = malloc(headsize);

	ptr[0] = 0; // == NULL
	ptr[1] = 0;
	ptr[2] = blocksize;

	size_t n = size / blocksize + (size % blocksize != 0);
	void *p = ptr + 3;
	for (int i = 0; i < n; i++) {
		void *mem;
		mem = malloc(len);
		*((void **)p) = mem;
		p = mem;
		p += blocksize;
	}
	*(void **)p = NULL;

	return ptr;
}

void *lrealloc(void *ptr, size_t size)
{
	size_t *s = (size_t *)ptr;
	// last accessed block
	void *lastblock = (void *)s[0]; // Not 100% clean, because requires sizeof(size_t) == sizeof(void **), is this guaranteed?
	size_t lastblock_idx = s[1];
	size_t blocksize = s[2];
	void *p;
	int idx;

	// Check if new block(s) need to be allocated
	size_t required_blocks = size / blocksize + (size % blocksize != 0) - 1;

	// Advance to required_blocks
	if (lastblock == NULL || required_blocks < lastblock_idx) {
		p = (void *)s[3];
		idx = 0;
	}
	else {
		p = lastblock;
		idx = lastblock_idx;
	}
	void **p_ = (void **)(p + blocksize);
	while (*p_ != NULL && idx < required_blocks) {
		idx += 1;
		p = *p_;
		p_ = (void **)(p + blocksize);
	}
	// Update last used block
	s[0] = (size_t) p;
	s[1] = idx;

	p += blocksize;

	// Free blocks if new size is smaller
	void *pp = p;
	while (*(void **)pp != NULL) {
		printf("%p %p\n", p, pp);
		void *pf;
		pf = *(void **)pp;
		pp = pf + blocksize;
		free(pf);
	}

	// Allocate new blocks
	for (int i = idx; i < required_blocks; i++) {
		void *mem;
		mem = malloc(blocksize+sizeof(void *));
		*((void **)p) = mem;
		p = mem + blocksize;
	}

	// Seal last block off
	*(void **)p = NULL;

	return ptr;
}

void *lptr(void *ptr, size_t stride, size_t i)
{
	size_t *s = (size_t *)ptr;
	// last accessed block
	void *lastblock = (void *)s[0];
	size_t lastblock_idx = s[1];
	size_t blocksize = s[2];

	// Compute block & index in block
	i *= stride;
	int block = i / blocksize;
	int inner = i % blocksize;

	// Get convenient pointer to block
	void *p;
	if (lastblock == NULL || lastblock_idx > block) p = (void *)s[3];
	else                                            p = lastblock;

	// Advance to block
	for (; lastblock_idx < block; lastblock_idx++) p = *((void **)(p + blocksize));

	return p + inner;
}

void lfree(void *ptr)
{
	size_t *s = (size_t *)ptr;
	size_t blocksize = s[2];
	void *p = (void *)s[3];

	while (p != NULL) {
		void *pf;
		pf = p;
		p = *(void **)(p + blocksize);
		free(pf);
	}

	free(ptr);
	return;
}

