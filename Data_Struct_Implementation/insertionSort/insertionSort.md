## Insertion Sort

![Animation](https://images2017.cnblogs.com/blog/849589/201710/849589-20171015225645277-1151100000.gif)

### **Complexity**

- Auxiliary Space: O(1)
- Best Case: O(n) (reverse sorted)
- Worst Case: O(n^2)

### Code (Array Implementation)
```c
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

typedef void (*sortAlgorithm)(int *array, int size);

void insertionSort(int *array, int size) {
    int i, j;
    int temp;

    for (i = 0; i < size; i++) {
        temp = array[i];
        for (j = i; j >= 1; j--) {
            if (array[j-1] <= temp) {
                break;
            } 
            array[j] = array[j-1];
        }
        array[j] = temp;
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
    sortAlgorithm sort_method = insertionSort;
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

### Code (Linked List Implementation)
[Leetcode 147](https://leetcode.com/problems/insertion-sort-list/)
```c
/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     struct ListNode *next;
 * };
 */

struct ListNode* insertionSortList(struct ListNode* head){
    struct ListNode *dummy = (struct ListNode *) calloc (1, sizeof(struct ListNode));
    
    while(head) {
        struct ListNode *nextElement = head->next;
        struct ListNode *iter = dummy;
        
        while(iter->next != NULL && iter->next->val < head->val) {
            iter = iter->next;
        }
        
        head->next = iter->next;
        iter->next = head;
        head = nextElement;
    }
    
    struct ListNode *result = dummy->next;
    free(dummy);
    
    return result;
}
```

