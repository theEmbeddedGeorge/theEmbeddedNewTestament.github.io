## Check if binary representation of a number is palindrome

Given an integer ‘x’, write a C function that returns true if binary representation of x is palindrome else return false.
For example a numbers with binary representation as 10..01 is palindrome and number with binary representation as 10..00 is not palindrome.

The idea is similar to checking a string is palindrome or not. We start from leftmost and rightmost bits and compare bits one by one. If we find a mismatch, then return false.

```c
#include<stdio.h>
#include<stdint.h>

typedef uint32_t (*generalTestFunct)(uint32_t target);

uint32_t isBinaryPanlidrome(uint32_t target) {
    int i = 0;
    for (i; i < 32; i++) {
        if (((target>>i) & 0x1) != ((target>>(32-1-i)) & 0x1))
            return 0;
    }

    return 1;
}

int main(void) {
    generalTestFunct test_func1 = isBinaryPanlidrome;
    
    // test 1:
    uint32_t test_num1 = 0;
    printf("%d\n", test_func1(test_num1));
    
    // test 2:
    test_num1 = 0xf000000f;
    printf("%d\n", test_func1(test_num1));
    
    // test 3:
    test_num1 = 0x11001100;
    printf("%d\n", test_func1(test_num1));

    // test 4:
    test_num1 = 0xa0000005;
    printf("%d\n", test_func1(test_num1));
    
    return 0;
}
```