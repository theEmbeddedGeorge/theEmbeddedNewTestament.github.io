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

#define my_sizeof(type) (char *)(&type+1)-(char*)(&type) 

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

int main() {
    Random_item tmp;
    Random_item2 tmp2;

    printf("my_sizeof the item is %ld sizeof: %ld\n", my_sizeof(tmp), sizeof(tmp));
    printf("my_sizeof the item is %ld sizeof: %ld\n", my_sizeof(tmp2), sizeof(tmp2));

    return 0;
}
```

#### Reference
https://www.geeksforgeeks.org/implement-your-own-sizeof/

https://www.geeksforgeeks.org/structure-member-alignment-padding-and-data-packing/


