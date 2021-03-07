## Multiply 16-bit number with 8-bit multipler

                                       [mHigh mLow]    X
                                       [nHigh nLow]
                                       —————————————
                        [mHigh * nLow] [mLow * nLow]
        [mHigh * nHigh] [mLow * nHigh]
    ——————————————————————————————————————————————————————————————–
    [mHigh * nHigh] + [mLow * nHigh + mHigh * nLow] + [mLow * nLow]
    ——————————————————————————————————————————————————————————————–


```c
#include<stdio.h>
#include<stdint.h>

typedef uint32_t (*generalTestFunct)(uint16_t AB, uint16_t CD);

uint16_t mult_8_bit(uint8_t A, uint8_t B) {
    return (uint16_t)(A * B);
} 

uint32_t mult_16_bit2(uint16_t m, uint16_t n)
{
    uint8_t mLow = (m & 0x00FF);              // stores first 8–bits of `m`
    uint8_t mHigh = (m & 0xFF00) >> 8;        // stores last 8–bits of `m`
    uint8_t nLow = (n & 0x00FF);              // stores first 8–bits of `n`
    uint8_t nHigh = (n & 0xFF00) >> 8;        // stores last 8–bits of `n`
 
    uint16_t mLow_nLow = mult_8_bit(mLow, nLow);
    uint16_t mHigh_nLow = mult_8_bit(mHigh, nLow);
    uint16_t mLow_nHigh = mult_8_bit(mLow, nHigh);
    uint16_t mHigh_nHigh = mult_8_bit(mHigh, nHigh);
 
    // return 32–bit result (don't forget to shift `mHigh_nLow` and `mLow_nHigh`
    // by 1 byte and `mHigh_nHigh` by 2 bytes)
    return mLow_nLow + ((uint32_t)(mHigh_nLow + mLow_nHigh) << 8) + ((uint32_t)mHigh_nHigh << 16);
}

int main(void) {
    generalTestFunct test_func1 = mult_16_bit2;
    
    // test 1:
    uint16_t test_num1 = 0xf;
    uint16_t test_num2 = 0x3;
    printf("Expect: %x Actual: %x \n", (uint32_t) test_num1*test_num2, test_func1(test_num1, test_num2));
    
    // test 2:
    test_num1 = 0x1ff;
    test_num2 = 0x1ff;
    printf("Expect: %x Actual: %x \n", (uint32_t) test_num1*test_num2, test_func1(test_num1, test_num2));
    
    // test 3:
    test_num1 = 0xffff;
    test_num2 = 0xffff;
    printf("Expect: %x Actual: %x \n", (uint32_t) test_num1*test_num2, test_func1(test_num1, test_num2));

    // test 4:
    test_num1 = 0x1fff;
    test_num2 = 0x3;
    printf("Expect: %x Actual: %x \n", (uint32_t) test_num1*test_num2, test_func1(test_num1, test_num2));
    
    return 0;
}
```

### Reference

[multiply-16-bit-integers-using-8-bit-multiplier](https://www.techiedelight.com/multiply-16-bit-integers-using-8-bit-multiplier/)