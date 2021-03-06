## Count number of bits to be flipped to convert A to B

Given two numbers ‘a’ and b’. Write a program to count number of bits needed to be flipped to convert ‘a’ to ‘b’.

```c
#include<stdio.h>
#include<stdint.h>

typedef uint32_t (*generalTestFunct)(uint32_t num1, uint32_t num2);

uint32_t flipBitsNumber(uint32_t num1, uint32_t num2) {
    num1 ^= num2;

    int res = 0;
    while(num1) {
        res ++;
        num1 &= num1-1;
    }

    return res;
}

int main(void) {
    generalTestFunct test_func1 = flipBitsNumber;
    
    // test 1:
    uint32_t test_num1 = 1;
    uint32_t test_num2 = 1;
    printf("%d\n", test_func1(test_num1, test_num2));
    
    // test 2:
    test_num1 = 0xffff0000;
    test_num2 = 0x1;
    printf("%d\n", test_func1(test_num1, test_num2));
    
    // test 3:
    test_num1 = 0x11001100;
    test_num2 = 1;
    printf("%d\n", test_func1(test_num1, test_num2));

    // test 4:
    test_num1 = 0x00110011;
    test_num2 = 1;
    printf("%d\n", test_func1(test_num1, test_num2));
    
    return 0;
}
```