## Generic Memory Pool Implementation

[A generic C memory pool implementation](https://github.com/jobtalle/pool)

### Important Data structure

```c
/*
This is the linked list data structure that keeps track of
free memory blocks in the pool.
*/
typedef struct poolFreed{
	struct poolFreed *nextFree;
} poolFreed;

/*
The memory pool data structure that contains metadata about the pool
and maintain a list of free blocks to be allocated.
*/
typedef struct {
	uint32_t elementSize;
	uint32_t blockSize;
	uint32_t used;
	int32_t block;
	poolFreed *freed;
	uint32_t blocksUsed;
	uint8_t **blocks;
} pool;
```

### Pool Implementation

```c

#ifndef max
#define max(a,b) ((a)<(b)?(b):(a))
#endif

/*
Pool Initialization function
*/
void poolInitialize(pool *p, const uint32_t elementSize, const uint32_t blockSize)
{
	uint32_t i;

	p->elementSize = max(elementSize, sizeof(poolFreed));
	p->blockSize = blockSize;
    
    // Clear the pool before any usage
	poolFreeAll(p);

	p->blocksUsed = POOL_BLOCKS_INITIAL; // 1
	p->blocks = malloc(sizeof(uint8_t*) * p->blocksUsed);

	for(i = 0; i < p->blocksUsed; ++i)
		p->blocks[i] = NULL;
}

void poolFreePool(pool *p)
{
	uint32_t i;
	for(i = 0; i < p->blocksUsed; ++i) {
		if(p->blocks[i] == NULL)
			break;
		else
			free(p->blocks[i]);
	}

	free(p->blocks);
}

#ifndef DISABLE_MEMORY_POOLING
void *poolMalloc(pool *p)
{
	if(p->freed != NULL) {
		void *recycle = p->freed;
		p->freed = p->freed->nextFree;
		return recycle;
	}

	if(++p->used == p->blockSize) {
		p->used = 0;
		if(++p->block == (int32_t)p->blocksUsed) {
			uint32_t i;

			p->blocksUsed <<= 1;
			p->blocks = realloc(p->blocks, sizeof(uint8_t*)* p->blocksUsed);

			for(i = p->blocksUsed >> 1; i < p->blocksUsed; ++i)
				p->blocks[i] = NULL;
		}

		if(p->blocks[p->block] == NULL)
			p->blocks[p->block] = malloc(p->elementSize * p->blockSize);
	}
	
	return p->blocks[p->block] + p->used * p->elementSize;
}

void poolFree(pool *p, void *ptr)
{
	poolFreed *pFreed = p->freed;

    // Insert the free block to the head of the free list
	p->freed = ptr;
	p->freed->nextFree = pFreed;
}
#endif

void poolFreeAll(pool *p)
{
	p->used = p->blockSize - 1;
	p->block = -1;
	p->freed = NULL;
}
```

### Advance Reading

[Writing a Pool Allocator I](http://dmitrysoshnikov.com/compilers/writing-a-memory-allocator/)

[Writing a Pool Allocator II](http://dmitrysoshnikov.com/compilers/writing-a-pool-allocator/)