#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef enum {
    FALSE = 0,
    TRUE,
} BOOL;


BOOL is_small_endian() {
	uint32_t num = 1;
        uint8_t byte;
        
        // examine the first byte memory of the 4-byte uint32_t
        byte = *((uint8_t*) &num);

	return (byte == 1) ? TRUE : FALSE;
}

int main (int argc, char *argv[]) {

	if (is_small_endian())
            printf("Systen is of small endian!\n");
        else
            printf("Systen is of big endian!\n");

	return 0;
}
