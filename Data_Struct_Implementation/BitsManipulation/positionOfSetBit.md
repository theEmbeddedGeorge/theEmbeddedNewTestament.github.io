```c
#include<stdio.h>
#include<stdint.h>

// Task: Find Bit position of the only set bit
// Tips: right shift until the number becomes zero. Be careful that the index starts with -1 instead of 0!
typedef int (*generalTestFunct)(uint32_t target);

int setBit_pos(uint32_t target) {
    int pos = -1;
    while (target) {
        pos ++;
        target >>= 1;
    }
    
    return pos;
}

int main(void) {
    generalTestFunct test_func = setBit_pos;
    
    // test 1:
    uint32_t test1 = 0x1;
    printf("%x\n", test_func(test1));
    
    // test 2:
    uint32_t test2 = 0x8;
    printf("%x\n", test_func(test2));
    
    // test 3:
    uint32_t test3 = 0x80;
    printf("%x\n", test_func(test3));
    
    // test 4:
    uint32_t test4 = 0x10000;
    printf("%x\n", test_func(test4));
    
    return 0;
}
```