#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>


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