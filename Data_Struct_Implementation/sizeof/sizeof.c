#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#define my_sizeof(type) (char*)(&type+1)-(char*)(&type) 

typedef struct Random_item {
    char a;
    int b;
    char c;
} Random_item;

typedef struct Random_item2 {
    char a;
    int b;
    char c;
    char d;
    uint16_t e;
} Random_item2;

typedef struct Random_item3 {
    int b;
    char d;
    long int a;
    int c;
    uint16_t f;
} Random_item3;

void argument_alignment_check( char c1, char c2 ) 
{ 
   // Considering downward stack 
   // (on upward stack the output will be negative) 
   printf("Displacement %ld\n", (char*)&c2 - (char*)&c1); 
} 

int main() {
    Random_item tmp;
    Random_item2 tmp2;
    Random_item3 tmp3;

    printf("my_sizeof the item is %ld sizeof: %ld\n", my_sizeof(tmp), sizeof(tmp));
    printf("my_sizeof the item is %ld sizeof: %ld\n", my_sizeof(tmp2), sizeof(tmp2));
    printf("my_sizeof the item is %ld sizeof: %ld\n", my_sizeof(tmp3), sizeof(tmp3));

    argument_alignment_check('a', 'b');

    return 0;
}
