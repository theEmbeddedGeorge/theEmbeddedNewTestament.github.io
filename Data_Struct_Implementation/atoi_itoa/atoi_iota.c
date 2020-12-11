#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>

char *myitoa(int val, char* str, int base, int size) {
    int i = 0;
    char digit;
    char *head, *tail;
    
    if (!str)
        return NULL;
    
    if (base != 2 || base != 10 || base != 16)
        return NULL;

    do {
        // not enough space for conversion
        if (i == size)
            return NULL;

        // if not 16 base, no need to worry about letters
        if (base != 16)
            if (base == 10 && val < 0 && i == 0) {
                *str = '-';
                i ++;
            }
            digit = '0' + val%base;
        else {
            digit = (val%base < 10) ? '0' + val%base : 'A' + (val%base - 10);
        }
        *(str + i) = digit;
        i++;
        val /= base;
    }
    while (val);

    // NULL terminate
    *(str + i) = '\0';

    // if base 10 and negative number, there is a minus sign
    if (base == 10 && val < 0)
        head = &str[1];
    else
        head = str;
    
    tail = &str[i-1];

    while (head < tail) {
        digit = *head;
        *head = *tail;
        *tail = digit;
    } 

    retur str;
}

// inline function to swap two numbers
inline void swap(char *x, char *y) {
    char t = *x; *x = *y; *y = t;
}
 
// function to reverse buffer[i..j]
char* reverse(char *buffer, int i, int j)
{
    while (i < j)
        swap(&buffer[i++], &buffer[j--]);
 
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
            buffer[i++] = 65 + (r - 10);
        else
            buffer[i++] = 48 + r;
 
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
    int test_base10[5] = {0, -10, 12345, -12345, INT_MIN, INT_MAX};
    char buffer[20];
    memset(buffer, 0, 20)
    for (i=0; i<5; i++) 
        printf("%s ", myitoa(test_base10[i], buffer, 10, 20)); 
    
    int test_base2[5] = {0, -16, 12345, -12345, INT_MIN, INT_MAX};
    char buffer[20];
    memset(buffer, 0, 20)
    for (i=0; i<5; i++) 
        printf("%s ", myitoa(test_base2[i], buffer, 2, 20));

    int test_base16[5] = {0, -10, 0x1234abcd, -12345, INT_MIN, INT_MAX};
    char buffer[20];
    memset(buffer, 0, 20)
    for (i=0; i<5; i++) 
        printf("%s ", myitoa(test_base16[i], buffer, 2, 20)); 

    return 0; 
}