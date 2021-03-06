```c
#include<stdio.h>
#include<stdint.h>

// Task: Swap all odd and even bits
// Tips: get the number with all odd bits and all even bits first and then or them together.
uint32_t swapOddEven(uint32_t target) {
    return ((target & 0xaaaaaaaa) >> 1) | ((target & 0x55555555) << 1);
}

int main(void) {
    // test 1:
    uint32_t test1 = 0xa;
    printf("%x\n", swapOddEven(test1));
    
    // test 2:
    uint32_t test2 = 0x5;
    printf("%x\n", swapOddEven(test2));
    
    // test 3:
    uint32_t test3 = 0xff;
    printf("%x\n", swapOddEven(test3));
    
    // test 4:
    uint32_t test4 = 0xaa;
    printf("%x\n", swapOddEven(test4));
    
    return 0;
}
```