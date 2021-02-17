## Bubble Sort

![Algorithm Animation](https://images2017.cnblogs.com/blog/849589/201710/849589-20171015223238449-2146169197.gif)

#### Complexity

- Worst and Average Case Time Complexity: O(n*n). Worst case occurs when array is reverse sorted.
- Best Case Time Complexity: O(n). Best case occurs when array is already sorted.
- Auxiliary Space: O(1)

#### Code
```c
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

typedef void (*sortAlgorithm)(int *array, int size);

void bubblesort(int *array, int size) {
    int i, j;
    int temp;

    for (i = size; i >= 0; i--) {
        for (j = 0; j < i-1; j++) {
            if (array[j] > array[j+1]) {
                temp = array[j];
                array[j] = array[j+1];
                array[j+1] = temp;
            }
        }
    }
}

void printArray(int arr[], int size) 
{ 
    int i; 
    for (i=0; i < size; i++) 
        printf("%d ", arr[i]); 
    printf("\n"); 
} 

void tests(int *nums, int size) {
    sortAlgorithm sort_method = bubblesort;
    clock_t start, end;
    double cpu_time_used;

    start = clock();
    sort_method(nums, size);
    end = clock();

    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    printf("==== Sorted array test results ====\n"); 
    printArray(nums, size); 
    printf("CPU time used: %f\n\n", cpu_time_used);
}

int main() {
    // test 1
    int nums[] = {10, 7, 8, 9, 1, 5}; 
    int n = sizeof(nums)/sizeof(nums[0]); 
    tests(nums, n);

    // test 2
    int nums2[] = {1, 2, 4, 6, 8, 2, 3, 4, 0, -1, 10, 7, 8, 9, 1, 5}; 
    n = sizeof(nums2)/sizeof(nums2[0]); 
    tests(nums2, n);
    
    // test 3
    int nums3[] = {}; 
    n = sizeof(nums3)/sizeof(nums3[0]); 
    tests(nums3, n);

    // test 4
    int nums4[] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1}; 
    n = sizeof(nums4)/sizeof(nums4[0]); 
    tests(nums4, n);
    
    return 0; 
}
```
