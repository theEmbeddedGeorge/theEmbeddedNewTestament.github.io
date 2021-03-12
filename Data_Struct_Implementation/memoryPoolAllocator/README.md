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
	uint32_t chunkSize; // size of each chunk
	uint32_t blockSize; // size of each block
	uint32_t used; // number of chunks in use in current block
	int32_t block; // current block index
	poolFreed *freed;  // overall free chunks list
	uint32_t blocksUsed; // number of block allocated
	uint8_t **blocks; // block memory
} pool;
```

### Pool Implementation

```c

#ifndef max
#define max(a,b) ((a)<(b)?(b):(a))
#endif

/*
Pool Initialization function. Pool is only able to allocate chunSize objects
*/
void poolInitialize(pool *p, const uint32_t chunkSize, const uint32_t blockSize)
{
	uint32_t i;

	// define chunk and block parammeters
	p->chunkSize = max(chunkSize, sizeof(poolFreed));
	p->blockSize = blockSize;
    
    // Clear the pool before any usage
	poolFreeAll(p);

	// assign initial number of blocks
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
			free(p->blocks[i]); // free this block
	}

	// free all block 
	free(p->blocks);
}

#ifndef DISABLE_MEMORY_POOLING
void *poolMalloc(pool *p)
{
	// if there is free chunk, reuse it
	if(p->freed != NULL) {
		void *recycle = p->freed;
		p->freed = p->freed->nextFree;
		return recycle;
	}

	// else, we proceed to the chunk in current block
	// if block is full
	if(++p->used == p->blockSize) {
		p->used = 0; // reset used chunk number to 0
		// if we also run out of blocks
		if(++p->block == (int32_t)p->blocksUsed) {
			uint32_t i;

			// assign 2 times block size
			p->blocksUsed <<= 1;
			p->blocks = realloc(p->blocks, sizeof(uint8_t*)* p->blocksUsed);

			// initialize to be NULL
			for(i = p->blocksUsed >> 1; i < p->blocksUsed; ++i)
				p->blocks[i] = NULL;
		}

		// assign memory for this free block
		if(p->blocks[p->block] == NULL)
			p->blocks[p->block] = malloc(p->chunkSize * p->blockSize);
	}
	
	// return the next free chunk address
	return p->blocks[p->block] + p->used * p->chunkSize;
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
	// reinitialize all 
	p->used = p->blockSize - 1;
	p->block = -1;
	p->freed = NULL;
}
```

### Advance Reading

[Writing a Pool Allocator I](http://dmitrysoshnikov.com/compilers/writing-a-memory-allocator/)

[Writing a Pool Allocator II](http://dmitrysoshnikov.com/compilers/writing-a-pool-allocator/)