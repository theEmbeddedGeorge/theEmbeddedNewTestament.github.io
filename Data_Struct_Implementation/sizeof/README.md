### my_sizeof implementation and sturcture padding

#### Usage
```
make
./stack
```

#### code
```C
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#define my_sizeof(type) (char*)(&type+1)-(char*)(&type) 

/* suppose this is a 32 bit machine, so a word (4 byte) alignment is required. */

// struct size 12 (3*sizeof(int)), p for padding bytes
typedef struct Random_item {
    char a; // 1
            // 3p
    int b;  // 4 -> dominant item
    char c; // 1
            // 3p to satisfy processor word alignment requirement
} Random_item;

// struct size 12
typedef struct Random_item2 {
    char a;    // 1
               // 3
    int b;     // 4 -> dominant item
    char c;    // 1
    char d;    // 1
    uint16_t e; // 2
} Random_item2;

// struct size 24 (3*sizeof(long int))
typedef struct Random_item3 {
    int b;      // 4
    char d;     // 1
                // 3p
    long int a; // 8 -> if long int is 8byte, then 64 bit machine
    int c;      // 4
    uint16_t f; // 2
                // 2p 
} Random_item3;

// struct size 32 (4*sizeof(long int))
typedef struct Random_item4 {
    int b;        // 4
    char d;       // 1
                  // 3p
    long int a;   // 8 -> if long int is 8byte, then 64 bit machine
    int c;        // 4
    uint16_t f;   // 2
    char e;       // 1
    char g;       // 1
    char h;       // 1
                  // 7p -> to align with 8 byte long int earilier
} Random_item4;

// struct size 56 
typedef struct Random_item5 {
    int b;        // 4
    char d;       // 1
                  // 3p
    long int a;   // 8 
    int c;        // 4
    uint16_t f;   // 2
    char e;       // 1
                  // 1p
    Random_item3 random; // 24  <--- we need to break the structure up, otherwise it is hard to judge the padding in between
    char g; // 1
    char h; // 1
            // 6p -> to align with 8 byte long int earilier
} Random_item5;

// should output 4 or -4
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

```

#### Notes

Predict the output of following program:

```C
#include <stdio.h> 
  
// Alignment requirements 
// (typical 32 bit machine) 
  
// char         1 byte 
// short int    2 bytes 
// int          4 bytes 
// double       8 bytes 
  
// structure A 
typedef struct structa_tag 
{ 
   char        c; 
   short int   s; 
} structa_t; 
  
// structure B 
typedef struct structb_tag 
{ 
   short int   s; 
   char        c; 
   int         i; 
} structb_t; 
  
// structure C 
typedef struct structc_tag 
{ 
   char        c; 
   double      d; 
   int         s; 
} structc_t; 
  
// structure D 
typedef struct structd_tag 
{ 
   double      d; 
   int         s; 
   char        c; 
} structd_t; 
  
int main() 
{ 
   printf("sizeof(structa_t) = %d\n", sizeof(structa_t)); 
   printf("sizeof(structb_t) = %d\n", sizeof(structb_t)); 
   printf("sizeof(structc_t) = %d\n", sizeof(structc_t)); 
   printf("sizeof(structd_t) = %d\n", sizeof(structd_t)); 
  
   return 0; 
} 
```

Answers:
```C
sizeof(structa_t) = 4
sizeof(structb_t) = 8
sizeof(structc_t) = 24
sizeof(structd_t) = 16
```

#### Reference
https://www.geeksforgeeks.org/implement-your-own-sizeof/

https://www.geeksforgeeks.org/structure-member-alignment-padding-and-data-packing/


