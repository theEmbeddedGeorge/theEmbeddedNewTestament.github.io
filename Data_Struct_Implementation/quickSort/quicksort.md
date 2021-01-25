## Circular Ring Buffer
#### Usage
```
make
./quicksort
```

#### Code
```c
#include <stdlib.h>
#include <stdio.h>

void quicksort(int nums[], int low, int high) {
    if(low >= high) {
        return;
    }
    
    int left = low, right = high;
    int pivot = nums[low + (right - low) / 2];

    while(left <= right) {
        while(left <= right && nums[left] < pivot) {
            left++;
        }

        while(left <= right && nums[right] > pivot) {
            right--;
        }

        if(left <= right) {
            int temp = nums[left];
            nums[left] = nums[right];
            nums[right] = temp;
            left++;
            right--;
        }
    }

    quicksort(nums, low, right);
    quicksort(nums, left, high);
}

void printArray(int arr[], int size) 
{ 
    int i; 
    for (i=0; i < size; i++) 
        printf("%d ", arr[i]); 
    printf("\n"); 
} 

int main() {
    int nums[] = {10, 7, 8, 9, 1, 5}; 
    int n = sizeof(nums)/sizeof(nums[0]); 

    quicksort(nums, 0, n-1); 
    printf("Sorted array: \n"); 
    printArray(nums, n); 
    return 0; 
}
```

