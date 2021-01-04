## Safe Memcpy that Handles Overlapped Src & Dest (Memmove)
#### Usage
```
make
./memcpy
```

#### Code
##### memcpy.h
```c
#define CPY_DIR_LOWER_TO_HIGHER 0
#define CPY_DIR_HIGHER_TO_LOWER 1
```

##### memcpy.c
```c
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "memcpy.h"
void *my_memmove(void *dest, const void *src, unsigned int n)
{
    char *pcSource =(char *)src;
    char *pcDstn =(char *)dest;

    // return if pcDstn or pcSource is NULL
    if ((pcSource == NULL)  || (pcDstn == NULL))
    {
        return NULL;
    }

    // overlap buffer
    if((pcSource < pcDstn) && (pcDstn < pcSource + n))
    {
        for (pcDstn += n, pcSource += n; n--;)
        {
            *--pcDstn = *--pcSource;
        }
    }
    else
    {
        while(n--)
        {
            *pcDstn++ = *pcSource++;
        }
    }
    return dest;
}

void myMemcpy(void *dest, void *src, size_t n){
    uint32_t cpyDir = CPY_DIR_LOWER_TO_HIGHER;
    char *pDest = (char *) dest;
    char *pSrc = (char *) src;

    if(pDest <= (pSrc + n) && (pSrc + n) <= (pDest + n) ) {
        cpyDir = CPY_DIR_HIGHER_TO_LOWER;
    }

    if(cpyDir == CPY_DIR_LOWER_TO_HIGHER) {
        for(int i = 0; i < n; i++) {
            *pDest++ = *pSrc++;
        }

        printf("copied from lower to higher\n");
    }
    else
    {
        for(int i = n - 1; i >= 0; i--) {
            *(pDest + i) = *(pSrc + i);
        }
        printf("copied from higher to lower\n");
    }
}

int main(int argc, char **argv) {
    char csrc[] = "iLoveEmbedded"; 
    char cdest[100]; 
    myMemcpy(cdest, csrc, strlen(csrc)+1); 
    printf("[1]Copied string is %s\n\n", cdest); 

    char csrc2[] = "iLoveEmbedded    "; 
    myMemcpy(csrc2 + 3, csrc2, strlen(csrc2)+1); 
    printf("[2]Copied string is %s\n\n", csrc2 + 3); 
    
    int isrc[] = {10, 20, 30, 40, 50}; 
    int n = sizeof(isrc)/sizeof(isrc[0]); 
    int idest[n], i; 
    myMemcpy(idest, isrc,  sizeof(isrc)); 
    printf("[3]Copied array is \n"); 
    for (i=0; i<n; i++) 
        printf("%d ", idest[i]); 
    return 0; 
}
```

#### Modified Proformance

found in GNU's newlib source code. I've posted the code here. If the source and destination pointers are both aligned on 4-byte boundaries, my modified-GNU algorithm copies 32 bits at a time rather than 8 bits. Listing 2 shows an implementation of this algorithm.

Listing 2: The modified-GNU algorithm
```C
void * memcpy(void * dst, void const * src, size_t len)
{
    long * plDst = (long *) dst;
    long const * plSrc = (long const *) src;

    if (!(src & 0xFFFFFFFC) && !(dst & 0xFFFFFFFC))
    {
        while (len >= 4)
    {
            *plDst++ = *plSrc++;
            len -= 4;
        }
    }

    char * pcDst = (char *) plDst;
    char const * pcDst = (char const *) plSrc;

    while (len–)
    {
        *pcDst++ = *pcSrc++;
    }

    return (dst);
}
```


#### Reference

https://www.embedded.com/optimizing-memcpy-improves-speed/

