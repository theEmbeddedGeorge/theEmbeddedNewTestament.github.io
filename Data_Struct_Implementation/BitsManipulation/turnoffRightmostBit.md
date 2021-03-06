## Turn off the rightmost set bit

Write a program that unsets the rightmost set bit of an integer.

```c
#include<stdio.h>
#include<stdint.h>

// Task:  Turn off the rightmost set bit
typedef uint32_t (*generalTestFunct)(uint32_t target);

uint32_t rightmostOff(uint32_t target) {
    return target & (target - 1);
}

int main(void) {
    generalTestFunct test_func = rightmostOff;
    
    // test 1:
    uint32_t test_num1 = 1;
    printf("%x\n", test_func(test_num1));
    
    // test 2:
    test_num1 = 0x1000;
    printf("%x\n", test_func(test_num1));
    
    // test 3:
    test_num1 = 0x1100;
    printf("%x\n", test_func(test_num1));
    
    // test 4:
    test_num1 = 0x100010;
    printf("%x\n", test_func(test_num1));
    
    return 0;
}
```