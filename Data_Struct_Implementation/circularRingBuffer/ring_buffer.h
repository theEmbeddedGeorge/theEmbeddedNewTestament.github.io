#include <stdio.h>
#include <stdlib.h>

#define RING_BUFFER_SIZE    24
#define BUFFER_IS_FULL      1
#define BUFFER_NOT_FULL     0
#define BUFFER_IS_EMPTY     1
#define BUFFER_NOT_EMPTY    0
#define ACCESS_SUCCESS      1
#define ACCESS_FAIL         0

typedef enum thread_idx {
    READ_THREAD_IDX = 0,
    WRITE_THREAD_IDX = 1,
    MAX_NUM_OF_THREADS = 2
} THREAD_IDX;

