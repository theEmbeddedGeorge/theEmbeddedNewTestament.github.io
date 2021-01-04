## Heap (Binary Implementation)
#### Usage
```
make
./memalign
```

#### Code
```c
#include <stdio.h>
#include <stdlib.h>

void *aligned_memory(size_t required, size_t alignment){
	void *p1;
	void **p2;

	p1 = malloc(alignment - 1 + sizeof(void *) + required);
	p2 = (void **)(((size_t)p1 + alignment - 1 + sizeof(void *)) & ~(alignment - 1));

	p2[-1] = p1;
	return p2;
}

void free_aligned(void *p) {
	free(((void **)p)[-1]);
}

int main (int argc, char *argv[]) {
	if(argc < 3) {
		printf("Wrong input\n");
		exit(1);
	}

	int required = atoi(argv[1]);
	int alignment = atoi(argv[2]);

	void *aligned_p = aligned_memory(required, alignment);

	printf("%p\n", aligned_p);

	return 0;
}

```
#### Analysis

![Alligned malloc](https://codeyarns.com/tech/2017-02-28-aligned-memory-allocation.png)

Since malloc is not guaranteed to align our memory for us, we’ll need to perform two extra steps:

1. Request extra bytes so we can returned an aligned address
2. Request extra bytes and store the offset between our original pointer and our aligned pointer

Consider the following:

- I call malloc and get a memory address X.
- I know I need to store a pointer offset value Y, which is fixed in size.
- Our alignment Z is variable.
- To handle this in a generic way, I always need to store alignment offset.
  - This is true even if the pointer is aligned
- When I allocate memory, X+Y (address + offset size) has the possibility to already be aligned, but it may also be unaligned
  - ***If X+Y is aligned, we would need no extra bytes***
  - ***If X+Y is unaligned, we would need Z-1 extra bytes in the worst case***

Example:
- Requested alignment 8
- malloc returns 0xF07
- we add two bytes for our offset storage, which brings us to 0xF09
- We need 7 extra bytes to get us to 0xF10.

Example #2 (let’s try to prove we don’t need 8):
- Requested alignment 8
- malloc returns 0xF06
- We add two bytes for our offset storage, bringing us to 0xF08
- We are now 8 byte aligned

So our worst case padding for malloc is:
```C
sizeof(offset_t) + (alignment - 1)
```

Which translates to our allocation as:
```C
uint32_t hdr_size = PTR_OFFSET_SZ + (align - 1);
void * p = malloc(size + hdr_size);
```


## Reference

https://embeddedartistry.com/blog/2017/02/22/generating-aligned-memory/

https://codeyarns.com/tech/2017-02-28-aligned-memory-allocation.html

