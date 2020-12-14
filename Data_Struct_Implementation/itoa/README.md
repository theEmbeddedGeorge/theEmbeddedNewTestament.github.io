## Safe Memcpy that Handles Overlapped Src & Dest (Memmove)
#### ***Usage***
```
make
./atoi
```

#### ***Notes***

This function is actually not easy as it seems. We need to consider the following:

1) When base is 10 and val is negative, need to include the negative sign '-'.
2) String order is reverse with digit order.
3) Remember to NULL terminate the string.
4) Handle val = 0 case.
5) When base is 16, handle special letter ABCDEF.
6) Remember to take abs(value) before modulation, other wise it will give a negative remainder.

#### ***Improvements***

The implementation below can be improved in the following ways:

1) Handle INT_MIN, currently it is unable to handle it.
2) Implementation below will seg fault if buffer does not have enough space to hold all digits. Maybe we could pass buffer length argument to the function.

#### ***Code***
###### itoa.c
```c
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>

// inline function to swap two numbers
inline static void my_swap(char *x, char *y) {
    char t = *x; *x = *y; *y = t;
}
 
// function to reverse buffer[i..j]
char* reverse(char *buffer, int i, int j)
{
    while (i < j)
        my_swap(&buffer[i++], &buffer[j--]);
 
    return buffer;
}
 
// Iterative function to implement itoa() function in C
char* itoa(int value, char* buffer, int base)
{
    // invalid input
    if (base < 2 || base > 32)
        return buffer;
 
    // consider absolute value of number
    int n = abs(value);
 
    int i = 0;
    while (n)
    {
        int r = n % base;
 
        if (r >= 10) 
            buffer[i++] = 'A' + (r - 10);
        else
            buffer[i++] = '0' + r;
 
        n = n / base;
    }
 
    // if number is 0
    if (i == 0)
        buffer[i++] = '0';
 
    // If base is 10 and value is negative, the resulting string 
    // is preceded with a minus sign (-)
    // With any other base, value is always considered unsigned
    if (value < 0 && base == 10)
        buffer[i++] = '-';
 
    buffer[i] = '\0'; // null terminate string
 
    // reverse the string and return it
    return reverse(buffer, 0, i - 1);
}

int main(int argc, char **argv) {
    int i;
    char buffer[64];
    int buf_s = 64;

    int test_base10[6] = {0, -10, 12345, -12345, -32768, INT_MAX};
    memset(buffer, 0, buf_s);
    for (i=0; i<6; i++) 
        printf("%s ", itoa(test_base10[i], buffer, 10));
    printf("\n==============\n");  
    
    int test_base2[6] = {0, -16, 12345, -12345, -32768, INT_MAX};
    memset(buffer, 0, buf_s);
    for (i=0; i<6; i++) 
        printf("%s ", itoa(test_base2[i], buffer, 2));
    printf("\n==============\n"); 

    int test_base16[6] = {0, -10, 0x1234abcd, -12345, -32768, INT_MAX};
    memset(buffer, 0, buf_s);
    for (i=0; i<6; i++)
        printf("%s ", itoa(test_base16[i], buffer, 16)); 
    printf("\n==============\n");  
    return 0; 
}
```

