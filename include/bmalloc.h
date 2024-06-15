
void * bmalloc(size_t size, size_t blocksize);
void * brealloc(void *ptr, size_t size);
void bfree(void *ptr);
void *bptr(void *ptr, size_t stride, size_t i);

void * lmalloc(size_t size, size_t blocksize);
void * lrealloc(void *ptr, size_t size);
void lfree(void *ptr);
void *lptr(void *ptr, size_t stride, size_t i);

#define bget(PTR, IDX)      ( *((typeof(PTR)) bptr(PTR, sizeof(*PTR), IDX)) )
#define bset(PTR, IDX, VAL) ( bget(PTR, IDX) = VAL )

// TODO: this is ugly
#define lget(PTR, IDX)      ( *((typeof(PTR)) lptr(PTR, sizeof(*PTR), IDX)) )
#define lset(PTR, IDX, VAL) ( lget(PTR, IDX) = VAL )

