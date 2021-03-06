## Reverse Bits

```c
#include<stdio.h>
#include<stdint.h>

typedef uint32_t (*generalTestFunct)(uint32_t target);

uint32_t reverseBits(uint32_t target) {
    target = ((target & 0xffff0000) >> 16) | ((target & 0x0000ffff) << 16);
    target = ((target & 0xff00ff00) >> 8) | ((target & 0x00ff00ff) << 8);
    target = ((target & 0xf0f0f0f0) >> 4) | ((target & 0x0f0f0f0f) << 4);
    target = ((target & 0xcccccccc) >> 2) | ((target & 0x33333333) << 2);
    target = ((target & 0xaaaaaaaa) >> 1) | ((target & 0x55555555) << 1);

    return target;
}

int main(void) {
    generalTestFunct test_func1 = reverseBits;
    
    // test 1:
    uint32_t test_num1 = 1;
    printf("%x\n", test_func1(test_num1));
    
    // test 2:
    test_num1 = 0xffff0000;
    printf("%x\n", test_func1(test_num1));
    
    // test 3:
    test_num1 = 0x11001100;
    printf("%x\n", test_func1(test_num1));

    // test 4:
    test_num1 = 0x00110011;
    printf("%x\n", test_func1(test_num1));
    
    return 0;
}
```