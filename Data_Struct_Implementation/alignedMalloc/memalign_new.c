#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

void *aligned_memory(size_t required, size_t alignment){

    void* ptr = (void*) malloc(required + (alignment - 1) + sizeof(void*));
    void** ret_ptr = (void**)((size_t)(ptr + sizeof(void*) + (alignment - 1)) & ~(alignment - 1));

    ret_ptr[-1] = ptr; 
    return ret_ptr;
}

void free_aligned(void *p) {
    void** _p = &p;
    
    free(_p[-1]);
}

int main (int argc, char *argv[]) {
	if(argc < 3) {
		printf("Wrong input\n");
		exit(1);
	}

	int required = atoi(argv[1]);
	int alignment = atoi(argv[2]);

	void *aligned_p = aligned_memory(required, alignment);
        void **_aligned_p = &aligned_p;

	printf("Aligned address: %p  Malloc address: %p\n", aligned_p, _aligned_p[-1]);

	return 0;
}
