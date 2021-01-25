## Quick Sort
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

#### Complexity

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

#### Why Quick Sort is preferred over MergeSort for sorting Arrays?

Quick Sort in its general form is an in-place sort (i.e. it doesn’t require any extra storage) whereas merge sort requires O(N) extra storage, N denoting the array size which may be quite expensive. Allocating and de-allocating the extra space used for merge sort increases the running time of the algorithm. Comparing average complexity we find that both type of sorts have O(NlogN) average complexity but the constants differ. For arrays, merge sort loses due to the use of extra O(N) storage space.

#### Why MergeSort is preferred over QuickSort for Linked Lists?

In case of linked lists the case is different mainly due to difference in memory allocation of arrays and linked lists. Unlike arrays, linked list nodes may not be adjacent in memory. Unlike array, in linked list, we can insert items in the middle in O(1) extra space and O(1) time. Therefore merge operation of merge sort can be implemented without extra space for linked lists.