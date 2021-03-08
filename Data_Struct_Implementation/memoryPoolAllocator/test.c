#include <stdio.h>
#include <stdlib.h>
#include "pool.h"

#define SUCCESS 0
#define FAILURE -1

int  test_pool(int element_size, int block_size)
{
	pool pool_ptr;
	int *test_ptr1 = NULL;
	int *test_ptr2 = NULL;
	
	/* init memory pool with given parameters */
	poolInitialize(&pool_ptr, element_size, block_size);
	
	/* allocate memory from memory pool */
	test_ptr1 = poolMalloc(&pool_ptr);
	test_ptr2 = poolMalloc(&pool_ptr);
	
	/* test allocated memory validity */
	if(!(test_ptr1 && test_ptr2))
	{
		printf("memory allocation failure \n");
		
		return FAILURE;
	}
	
	/* free memory allocated from memory pool */
	poolFree(&pool_ptr, test_ptr1);
	
	/* free all memory used by this pool */
	poolFreePool(&pool_ptr);
	
	return SUCCESS;
}

int main()
{
	if(test_pool(4, 8) == FAILURE)
		printf("test_pool failure %s %d \n", __FILE__, __LINE__);
	
	if(test_pool(8, 8) == FAILURE)
		printf("test_pool failure %s %d \n", __FILE__, __LINE__);
	
	if(test_pool(16, 8) == FAILURE)
		printf("test_pool failure %s %d \n", __FILE__, __LINE__);

	if(test_pool(32, 8) == FAILURE)
		printf("test_pool failure %s %d \n", __FILE__, __LINE__);
  

	return SUCCESS;
}