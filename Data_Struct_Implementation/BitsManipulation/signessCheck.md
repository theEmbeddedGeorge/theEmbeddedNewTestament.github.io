## Detect if two integers have opposite signs

Given two signed integers, write a function that returns true if the signs of given integers are different, otherwise false. For example, the function should return true -1 and +100, and should return false for -100 and -200. The function should not use any of the arithmetic operators.

```c++
#include<stdio.h>
#include<stdint.h>

// Task:  Find position of the only set bit
typedef int (*generalTestFunct)(int target, int target2);

int checkSignDiff(int num1, int num2) {
    return (num1 ^ num2) < 0;
}

int main(void) {
    generalTestFunct test_func = checkSignDiff;
    
    // test 1:
    int test_num1 = 1;
    int test_num2 = -1;
    printf("%x\n", test_func(test_num1, test_num2));
    
    // test 2:
    test_num1 = 1;
    test_num2 = 1;
    printf("%x\n", test_func(test_num1, test_num2));
    
    // test 3:
    test_num1 = 2121312;
    test_num2 = 2121;
    printf("%x\n", test_func(test_num1, test_num2));
    
    // test 4:
    test_num1 = -213;
    test_num2 = 21443212;
    printf("%x\n", test_func(test_num1, test_num2));
    
    return 0;
}
```