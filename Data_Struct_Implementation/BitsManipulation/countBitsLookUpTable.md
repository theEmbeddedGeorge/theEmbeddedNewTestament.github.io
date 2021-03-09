## Count Set Bits with lookup table

To have a lookup table is a very fast way to check values. This is extremely useful for questions like [***Program to count number of set bits in an (big) array***](https://www.geeksforgeeks.org/program-to-count-number-of-set-bits-in-an-big-array/).

Basiclly we have an array of 256 entrys that record the number of set bits for 0 - 255 (Cover 8-bit representation). Then we could simply break 32-bit number into 4 parts and use the table to check for each 8 bit and sum it up.

```c++
#include <bits/stdc++.h>
using namespace std;
 
int BitsSetTable256[256];
 
// Function to initialise the lookup table 
void initialize() 
{ 
 
    // To initially generate the 
    // table algorithmically 
    BitsSetTable256[0] = 0; 
    for (int i = 0; i < 256; i++)
    { 
        BitsSetTable256[i] = (i & 1) + 
            BitsSetTable256[i / 2]; 
    } 
} 
 
// Function to return the count 
// of set bits in n 
int countSetBits(int n) 
{ 
    unsigned char *num = (unsigned char *) &n;
    return (BitsSetTable256[num[0]] + 
            BitsSetTable256[num[1]] + 
            BitsSetTable256[num[2]] + 
            BitsSetTable256[num[3]]);

    /*return (BitsSetTable256[n & 0xff] + 
            BitsSetTable256[(n >> 8) & 0xff] + 
            BitsSetTable256[(n >> 16) & 0xff] + 
            BitsSetTable256[n >> 24]);*/
} 
 
// Driver code 
int main() 
{ 
    // Initialise the lookup table 
    initialize(); 
    int n = 9; 
    cout << countSetBits(n);
} 
```