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
