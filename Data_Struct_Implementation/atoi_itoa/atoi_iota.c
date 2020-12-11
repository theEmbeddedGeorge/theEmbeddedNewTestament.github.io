#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>

char *myitoa(int val, char* str, int base) {
    int i = 0, isNegative = 0;
    char digit;
    char *head, *tail;
    
    if (!str)
        return NULL;
    
    if (base != 2 && base != 10 && base != 16)
        return NULL;

    if (base == 10 && val < 0) {
        isNegative = 1;
        val = -val;
    }

    if (val < 0)
        val = -val;

    do {
        // if not 16 base, no need to worry about letters
        digit = (val%base < 10) ? '0' + val%base : 'A' + (val%base - 10);
        *(str + i) = digit;
        i++;
        val /= base;
    }
    while (val);

    if (isNegative) {
        *(str + i) = '-';
        i ++;
    }

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
        head ++;
        tail --;
    } 

    return str;
}

int main(int argc, char **argv) {
    int i;
    char buffer[20];

    int test_base10[6] = {0, -10, 12345, -12345, INT_MIN, INT_MAX};
    memset(buffer, 0, 20);
    for (i=0; i<6; i++) 
        printf("%s ", myitoa(test_base10[i], buffer, 10));
    printf("\n==============\n");  
    
    int test_base2[6] = {0, -16, 12345, -12345, INT_MIN, INT_MAX};
    memset(buffer, 0, 20);
    for (i=0; i<6; i++) 
        printf("%s ", myitoa(test_base2[i], buffer, 2));
    printf("\n==============\n"); 

    int test_base16[6] = {0, -10, 0x1234abcd, -12345, INT_MIN, INT_MAX};
    memset(buffer, 0, 20);
    for (i=0; i<6; i++)
        printf("%s ", myitoa(test_base16[i], buffer, 16)); 
    printf("\n==============\n");  
    return 0; 
}