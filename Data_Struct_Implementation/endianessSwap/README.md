## Endianess Conversion
#### Usage
```
make
./endianess
```

### Code
```c
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

class Solution {
public:
    uint32_t reverseBits(uint32_t n) {
        n = (n << 16) | (n >> 16);
        n = ((n << 8) & 0xff00ff00) | ((n >> 8) & 0x00ff00ff);
        n = ((n << 4) & 0xf0f0f0f0) | ((n >> 4) & 0x0f0f0f0f);
        n = ((n << 2) & 0xcccccccc) | ((n >> 2) & 0x33333333);
        n = ((n << 1) & 0xaaaaaaaa) | ((n >> 1) & 0x55555555);
        
        return n;
    }
};
class Solution {
public:
    uint32_t reverseBits(uint32_t n) {
        uint32_t ret{};
        int m = 32;
        
        while(m --) {
            ret <<= 1;
            ret |= (n & 0x1);
            n >>= 1;
        }
        
        return ret;
    }
};
```

## Reference

https://www.geeksforgeeks.org/little-and-big-endian-mystery/


