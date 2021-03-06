## Swap bits in a given number

Given a number x and two positions (from the right side) in the binary representation of x, write a function that swaps n bits at given two positions and returns the result. It is also given that the two sets of bits do not overlap.

    1) Move all bits of the first set to the rightmost side
    set1 =  (x >> p1) & ((1U << n) - 1)
    Here the expression (1U << n) - 1 gives a number that 
    contains last n bits set and other bits as 0. We do & 
    with this expression so that bits other than the last 
    n bits become 0.

    2) Move all bits of second set to rightmost side
    set2 =  (x >> p2) & ((1U << n) - 1)

    3) XOR the two sets of bits
    xor = (set1 ^ set2) 

    4) Put the xor bits back to their original positions. 
    xor = (xor << p1) | (xor << p2)

    5) Finally, XOR the xor with original number so 
    that the two sets are swapped.
    result = x ^ xor

```c
#include <stdio.h>
// C program to swap bits in an integer
#include<stdio.h>
 
// This function swaps bit at positions p1 and p2 in an integer n
int swapBit(unsigned int n, unsigned int p1, unsigned int p2)
{
    /* Move p1'th to rightmost side */
    unsigned int bit1 =  (n >> p1) & 1;
 
    /* Move p2'th to rightmost side */
    unsigned int bit2 =  (n >> p2) & 1;
 
    /* XOR the two bits */
    unsigned int x = (bit1 ^ bit2);
 
    /* Put the xor bit back to their original positions */
    x = (x << p1) | (x << p2);
 
    /* XOR 'x' with the original number so that the
       two sets are swapped */
    unsigned int result = n ^ x;
}
 
/* Driver program to test above function*/
int main()
{
    int res =  swapBits(28, 0, 3);
    printf("Result = %d ", res);
    return 0;
}

int swapBits(unsigned int x, unsigned int p1, unsigned int p2, unsigned int n)
{
    /* Move all bits of first set to rightmost side */
    unsigned int set1 = (x >> p1) & ((1U << n) - 1);
 
    /* Move all bits of second set to rightmost side */
    unsigned int set2 = (x >> p2) & ((1U << n) - 1);
 
    /* XOR the two sets */
    unsigned int xor = (set1 ^ set2);
 
    /* Put the xor bits back to their original positions */
    xor = (xor << p1) | (xor << p2);
 
    /* XOR the 'xor' with the original number so that the 
       two sets are swapped */
    unsigned int result = x ^ xor;
 
    return result;
}
 
/* Driver program to test above function*/
int main()
{
    int res = swapBits(28, 0, 3, 2);
    printf("\nResult = %d ", res);
    return 0;
}
```