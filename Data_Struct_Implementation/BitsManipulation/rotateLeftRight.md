## Rotate bits of a number

Bit Rotation: A rotation (or circular shift) is an operation similar to shift except that the bits that fall off at one end are put back to the other end.
In left rotation, the bits that fall off at left end are put back at right end.

In right rotation, the bits that fall off at right end are put back at left end.

```c
#include<stdio.h>
#include<stdint.h>

// Task:  Turn off the rightmost set bit
typedef uint32_t (*generalTestFunct)(uint32_t target, int pos);

uint32_t leftRotate(uint32_t target, int pos) {
    return (target >> (32 - (pos%32))) | (target << (pos%32));
}

uint32_t rightRotate(uint32_t target, int pos) {
    return (target >> (pos%32)) | (target << (32-(pos%32)));
}

int main(void) {
    generalTestFunct test_func1 = leftRotate;
    generalTestFunct test_func2 = rightRotate;
    
    // test 1:
    uint32_t test_num1 = 1;
    int shift = 33;
    printf("%x\n", test_func1(test_num1, shift));
    printf("%x\n", test_func2(test_num1, shift));
    
    // test 2:
    test_num1 = 0x1000;
    shift = 1;
    printf("%x\n", test_func1(test_num1, shift));
    printf("%x\n", test_func2(test_num1, shift));
    
    // test 3:
    test_num1 = 0x1100;
    shift = 1;
    printf("%x\n", test_func1(test_num1, shift));
    printf("%x\n", test_func2(test_num1, shift));
    
    // test 4:
    test_num1 = 0x100010;
    shift = 1;
    printf("%x\n", test_func1(test_num1, shift));
    printf("%x\n", test_func2(test_num1, shift));
    
    return 0;
}
```