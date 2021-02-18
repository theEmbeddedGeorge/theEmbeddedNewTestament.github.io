#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>

int my_atoi(char* num, int size, int base) {
    int i, ret = 0;
    int neg = 0;
    
    if (!num || size == 0)
        return ret;
    
    // take care of negative sign if any
    if (num[0] == '-') {
        neg = 1;
        i = 1;
    } else {
        i = 0;
    }

    // take care of different bases
    for (i; i < size; i++) {
        ret *= base;
        if (base != 16)
            ret += num[i] - '0';
        else { // special handling for Hex numbers
            if (num[i] >= 'a' && num[i] <= 'f')
                ret += num[i] - 'a' + 10;
            else if (num[i] >= 'A' && num[i] <= 'F')
                ret += num[i] - 'A' + 10;
            else if (num[i] >= '0' && num[i] <= '9')
                ret += num[i] - '0';
        } 
    }

    return neg ? -ret : ret;
}

int main(int argc, char **argv) {
    int ret;

    char* test_base10 = "123456";
    printf("my: %d expect: %d", my_atoi(test_base10, strlen(test_base10), 10), atoi(test_base10));
    printf("\n==============\n");  
    
    test_base10 = "-123456";
    printf("my: %d expect: %d ", my_atoi(test_base10, strlen(test_base10), 10), atoi(test_base10));
    printf("\n==============\n");  

    char *test_base2 = "1000";
    printf("%d ", my_atoi(test_base2, strlen(test_base2), 2));
    printf("\n==============\n");

    test_base2 = "111";
    printf("%d ", my_atoi(test_base2, strlen(test_base2), 2));
    printf("\n==============\n");

    char *test_base16 = "1a";
    printf("%d ", my_atoi(test_base16, strlen(test_base16), 16));
    printf("\n==============\n");

    test_base16 = "100";
    printf("%d ", my_atoi(test_base16, strlen(test_base16), 16));
    printf("\n==============\n");

    test_base16 = "ff";
    printf("%d ", my_atoi(test_base16, strlen(test_base16), 16));
    printf("\n==============\n");

    test_base16 = "1A";
    printf("%d ", my_atoi(test_base16, strlen(test_base16), 16));
    printf("\n==============\n");

    test_base16 = "FF";
    printf("%d ", my_atoi(test_base16, strlen(test_base16), 16));
    printf("\n==============\n");
    return 0; 
}