#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

uint32_t endianess_swap(uint32_t num) {
    num = ((num & 0xffff0000) >> 16) | ((num & 0x0000ffff) << 16);
    num = ((num & 0xff00ff00) >> 8) | ((num & 0x00ff00ff) << 8);
    return num;
}

int main (int argc, char *argv[]) {

	uint32_t num = 0x12345678;
    uint8_t *n = (uint8_t*) &num;
    int i;

    for (i = 0; i < sizeof(num); i++) {
        printf("%x ", *n++);
    }
    printf("\n");

    uint32_t swap_num = endianess_swap(num);
    n = (uint8_t*) &swap_num;

    for (i = 0; i < sizeof(swap_num); i++) {
        printf("%x ", *n++);
    }
    printf("\n");

	return 0;
}
