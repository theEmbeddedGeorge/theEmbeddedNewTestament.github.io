## Bits Array
### Usage
```
make
./bitsArray
```

### Code
```c
#include <stdio.h>

#define SetBit(A, k) (A[k/32] |= (1 << k%32))
#define ClearBit(A, k) (A[k/32] &= ~(1 << k%32))
#define TestBit(A, k) ((A[k/32] & (1 << k%32)))

int main( int argc, char* argv[] )
{
   int A[10] = {0};
   int i;

   for ( i = 0; i < 10; i++ )
      A[i] = 0;                    // Clear the bit array

   printf("Set bit poistions 100, 200 and 300\n");
   SetBit( A, 100 );               // Set 3 bits
   SetBit( A, 200 );
   SetBit( A, 300 );

   // Check if SetBit() works:
   for ( i = 0; i < 320; i++ )
      if ( TestBit(A, i) )
         printf("Bit %d was set !\n", i);

   printf("\nClear bit poistions 200 \n");
   ClearBit( A, 200 );

   // Check if ClearBit() works:
   for ( i = 0; i < 320; i++ )
      if ( TestBit(A, i) )
         printf("Bit %d was set !\n", i);
}

```

## Reference

[Array of bits introduction](http://www.mathcs.emory.edu/~cheung/Courses/255/Syllabus/1-C-intro/bit-array.html)


