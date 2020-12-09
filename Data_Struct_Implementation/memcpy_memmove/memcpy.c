#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "memcpy.h"

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