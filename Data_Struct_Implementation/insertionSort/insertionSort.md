## Insertion Sort

#### Code (Linked List Implementation)
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

#### Complexity

- Auxiliary Space: O(1)
- Best Case: O(n) (reverse sorted)
- Worst Case: O(n^2)