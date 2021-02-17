#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

typedef void (*sortAlgorithm)(int *array, int size);

static void printArray(int arr[], int size) 
{ 
    int i; 
    for (i=0; i < size; i++) 
        printf("%d ", arr[i]); 
    printf("\n"); 
} 

static void merge_helper(int *array, int st_a, int st_b, int size) {
    int arr[size];
    
    int i = st_a;
    int j = st_b;
    int k = 0;
    
    while (i < st_b && j < (st_a + size)) {
        arr[k++] = (array[i] > array[j]) ? array[j++] : array[i++];
    }

    while (i < st_b)
        arr[k++] = array[i++];
    
    while (j < (st_a + size))
        arr[k++] = array[j++];
    
    for (k = 0; k < size; k++)
        array[st_a++] = arr[k];
}

static void sort_helper(int *array, int st, int end) {
    int mid = (end-st)/2 + st;
    int temp;

    if (st < end) {
        sort_helper(array, st, mid);
        sort_helper(array, mid+1, end);
        merge_helper(array, st, mid+1, end-st+1);
    }
}

void mergesort(int *array, int size) {
    sort_helper(array, 0, size-1);
}

void tests(int *nums, int size) {
    sortAlgorithm sort_method = mergesort;
    clock_t start, end;
    double cpu_time_used;

    printf("==== Sorted array test results ====\n");
    printf("Original:\n");
    printArray(nums, size); 

    start = clock();
    sort_method(nums, size);
    end = clock();

    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    printf("Sorted:\n");
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

    // test 5
    int nums5[] = {1, 2, -4, 6, -8, 2, 3, -4, 0, -1, 10, -1, 5}; 
    n = sizeof(nums5)/sizeof(nums5[0]); 
    tests(nums5, n);
    
    return 0; 
}