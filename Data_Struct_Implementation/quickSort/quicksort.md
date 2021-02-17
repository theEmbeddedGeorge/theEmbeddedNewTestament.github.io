## Quick Sort

![Animation](https://images2017.cnblogs.com/blog/849589/201710/849589-20171015230936371-1413523412.gif)

### **Complexity**

- Space: O(n)
- Best Case: O(nlog(n))
- Worst Case: All elements equal, O(n^2)
- Average: O(nlog(n))

Time taken by QuickSort in general can be written as following.

    T(n) = T(k) + T(n-k-1) + theta(n)

The first two terms are for two recursive calls, the last term is for the partition process. k is the number of elements which are smaller than pivot.

The time taken by QuickSort depends upon the input array and partition strategy. Following are three cases.

**Worst Case**: The worst case occurs when the partition process always picks greatest or smallest element as pivot. If we consider above partition strategy where last element is always picked as pivot, the worst case would occur when the array is already sorted in increasing or decreasing order. Following is recurrence for worst case.

    T(n) = T(0) + T(n-1) + theta(n)

which is equivalent to  

    T(n) = T(n-1) + theta(n)

The solution of above recurrence is theta(n2).

**Best Case**: The best case occurs when the partition process always picks the middle element as pivot. Following is recurrence for best case.

    T(n) = 2T(n/2) + theta(n)

The solution of above recurrence is \theta(nLogn). It can be solved using case 2 of Master Theorem.

**Average Case**:
To do average case analysis, we need to consider all possible permutation of array and calculate time taken by every permutation which doesn’t look easy.

We can get an idea of average case by considering the case when partition puts O(n/9) elements in one set and O(9n/10) elements in other set. Following is recurrence for this case.

    T(n) = T(n/9) + T(9n/10) + \theta(n)

Solution of above recurrence is also O(nLogn)

Although the worst case time complexity of QuickSort is O(n2) which is more than many other sorting algorithms like Merge Sort and Heap Sort, QuickSort is faster in practice, because its inner loop can be efficiently implemented on most architectures, and in most real-world data. QuickSort can be implemented in different ways by changing the choice of pivot, so that the worst case rarely occurs for a given type of data. However, merge sort is generally considered better when data is huge and stored in external storage.

### **Why Quick Sort is preferred over MergeSort for sorting Arrays?**

Quick Sort in its general form is an in-place sort (i.e. it doesn’t require any extra storage) whereas merge sort requires O(N) extra storage, N denoting the array size which may be quite expensive. Allocating and de-allocating the extra space used for merge sort increases the running time of the algorithm. Comparing average complexity we find that both type of sorts have O(NlogN) average complexity but the constants differ. For arrays, merge sort loses due to the use of extra O(N) storage space.

### **Why MergeSort is preferred over QuickSort for Linked Lists?**

In case of linked lists the case is different mainly due to difference in memory allocation of arrays and linked lists. Unlike arrays, linked list nodes may not be adjacent in memory. Unlike array, in linked list, we can insert items in the middle in O(1) extra space and O(1) time. Therefore merge operation of merge sort can be implemented without extra space for linked lists.

### Code
```c
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

void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

static void sort_helper(int *array, int st, int end) {
    if (st >= end)
        return;

    int left = st;
    int right = end;

    // No need to swap the pivot pos at the end if we choose mid as the pivot 
    int pivot = array[left + (right-left)/2];

    while(left < right) {
        while(left < right && array[left] < pivot)
            left ++;

        while(left < right && array[right] > pivot)
            right --;

        if (left < right) {
            swap(&array[left++], &array[right--]);
        }
    }

    sort_helper(array, st, left-1);
    sort_helper(array, right+1, end);
}

void quicksort(int *array, int size) {
    sort_helper(array, 0, size-1);
}

void tests(int *nums, int size) {
    sortAlgorithm sort_method = quicksort;
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
```

