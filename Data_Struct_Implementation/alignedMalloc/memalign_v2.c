#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

#define MY_SIZEOF(type) ((char*)(&type+1) - (char*)&type)

typedef struct {
	int a;
	int b;
	double c;
	char str[10];
} EXAMPLE1, *pEXAMPLE1;

typedef struct {
	char str[10];
	double nums[5];
	int a;
	int b;
	double c;
} EXAMPLE2, *pEXAMPLE2;

void* my_aligned_malloc(size_t size, size_t alignment) {
	void *old_ptr;
	void **p;
	uintptr_t offset;

	// allocate enough space for size + offset + alignment
	old_ptr = malloc(size+sizeof(void*)+(alignment-1)); 
	p = (void**) (((size_t)old_ptr + sizeof(void*) + (alignment - 1)) & ~(alignment-1));

	p[-1] = old_ptr;
	printf("new_ptr address: %p alignment: %d\n", p, alignment);
	printf("malloc addr: %p\n", old_ptr);

	return p;
}

void my_aligned_free(void* ptr) {
	void** p;

	p = ptr;

	printf("free old_ptr address at: %p new_ptr: %p\n", p[-1], p);
	free(p[-1]);
}

int main(int argc, int** argv){
	EXAMPLE1 a;
	EXAMPLE2 b;
	void *old_mem1, *old_mem2;
	void *new_mem1, *new_mem2;

	printf("EXAMPLE1 size: %d\n", (int) MY_SIZEOF(a));
	printf("EXAMPLE2 size: %d\n", (int) MY_SIZEOF(b));

	old_mem1 = (pEXAMPLE1) malloc(MY_SIZEOF(a));
	old_mem2 = (pEXAMPLE2) malloc(MY_SIZEOF(b));

	printf("old_mem1 address: %p old_mem2 address: %p\n", (void*)old_mem1, (void*)old_mem2);

	free(old_mem1);
	free(old_mem2);

	new_mem1 = (void*) my_aligned_malloc(MY_SIZEOF(a), 4);
	new_mem2 = (void*) my_aligned_malloc(MY_SIZEOF(b), 72);

	printf("new_mem1 address: %p new_mem2 address: %p\n", (void*)new_mem1, (void*)new_mem2);
	
	my_aligned_free(new_mem1);
	my_aligned_free(new_mem2);
}


