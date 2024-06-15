
void * bmalloc(size_t size, size_t blocksize);
void * brealloc(void *ptr, size_t size);
void bfree(void *ptr);
void *bptr(void *ptr, size_t stride, size_t i);

#define bget(PTR, IDX)      ( *((typeof(PTR)) bptr(PTR, sizeof(*PTR), IDX)) )
#define bset(PTR, IDX, VAL) ( bget(PTR, IDX) = VAL )

