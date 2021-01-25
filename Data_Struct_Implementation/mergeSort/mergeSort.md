## Merge Sort


#### Code (Array Version)
```c
void merge(int arr[], int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;
 
    /* create temp arrays */
    int L[n1], R[n2];
 
    /* Copy data to temp arrays L[] and R[] */
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];
 
    /* Merge the temp arrays back into arr[l..r]*/
    i = 0; // Initial index of first subarray
    j = 0; // Initial index of second subarray
    k = l; // Initial index of merged subarray
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        }
        else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
 
    /* Copy the remaining elements of L[], if there
    are any */
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }
 
    /* Copy the remaining elements of R[], if there
    are any */
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}
 
/* l is for left index and r is right index of the
sub-array of arr to be sorted */
void mergeSort(int arr[], int l, int r)
{
    if (l < r) {
        // Same as (l+r)/2, but avoids overflow for
        // large l and h
        int m = l + (r - l) / 2;
 
        // Sort first and second halves
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
 
        merge(arr, l, m, r);
    }
}
 
/* UTILITY FUNCTIONS */
/* Function to print an array */
void printArray(int A[], int size)
{
    int i;
    for (i = 0; i < size; i++)
        printf("%d ", A[i]);
    printf("\n");
}
 
/* Driver code */
int main()
{
    int arr[] = { 12, 11, 13, 5, 6, 7 };
    int arr_size = sizeof(arr) / sizeof(arr[0]);
 
    printf("Given array is \n");
    printArray(arr, arr_size);
 
    mergeSort(arr, 0, arr_size - 1);
 
    printf("\nSorted array is \n");
    printArray(arr, arr_size);
    return 0;
}
```

#### Code (Linked List Version)
[Leetcode 148](https://leetcode.com/problems/sort-list/)
```c
/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     struct ListNode *next;
 * };
 */

struct ListNode *mergeList(struct ListNode *a, struct ListNode *b) {
    if(a == b) {
        return a;
    }
    
    struct ListNode *dummy = (struct ListNode *) malloc (sizeof(struct ListNode));
    struct ListNode *result = dummy;
    
    while(a && b) {
        if(a->val < b->val) {
            dummy->next = a;
            a = a->next;
        }
        else {
            dummy->next = b;
            b = b->next;
        }
        
        dummy = dummy->next;
    }
    
    if(a) {
        dummy->next = a;
    } 
    else if(b) {
        dummy->next = b;
    }
    
    struct ListNode *temp = result;
    result = result->next;
    free(temp);
    
    return result;
}

struct ListNode* sortList(struct ListNode* head){
    if(head == NULL) {
        return NULL;
    }
    
    struct ListNode *fast = head;
    struct ListNode *slow = head;
    struct ListNode *pre = head;
    
    
    while(fast && fast->next) {
        fast = fast->next->next;
        pre = slow;
        slow = slow->next;
    }
    
    pre->next = NULL;
    
    
    if(head == slow) {
        return head;
    }
    
    return mergeList(sortList(head), sortList(slow));
}
```

#### Complexity
- Worst Case Time Complexity: O(nLogn)
- Average Case Time Complexity: O(nLogn)
- Best Case Time Complexity: O(nLogn)
- Auxiliary Space: O(n)

Time Complexity: Sorting arrays on different machines. Merge Sort is a recursive algorithm and time complexity can be expressed as following recurrence relation. 
T(n) = 2T(n/2) + θ(n)

The above recurrence can be solved either using the Recurrence Tree method or the Master method. It falls in case II of Master Method and the solution of the recurrence is θ(nLogn). Time complexity of Merge Sort is  θ(nLogn) in all 3 cases (worst, average and best) as merge sort always divides the array into two halves and takes linear time to merge two halves.
Auxiliary Space: O(n)