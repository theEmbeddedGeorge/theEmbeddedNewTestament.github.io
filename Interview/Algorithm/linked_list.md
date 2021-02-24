## Problems

Index # | Title | Diffculty | Importance/Frequency
----|----|----|---
1 | Reverse Linked List | Easy | *****
2 | Reverse Linked List II | Medium | ***
3 | Linked List Cycle | Easy | *****
4 | Palindrome Linked List | Easy | ****
5 | Remove Linked List Elements | Easy | ****
6 | Delete Node in a Linked List | Easy | *****
7 | Remove Duplicates from Sorted List | Easy | *****
8 | Merge Two Sorted Lists | Easy | *****
9 | Flatten Binary Tree to Linked List | Medium | **
10 | Intersection of Two Linked Lists | Easy | ****
11 | LRU cache | Hard | ****
12 | Middle of linked list | Easy | ****
13 | Implement queue by linked list | Easy | ****
14 | Reorder List | Medium | ****

## Implementation

### **Reorder List**

***Big O:*** O(n) speed, O(1) space
```
Tips: 

1. Find a Middle Node.
2. Reverse the Second Part of the List.
3. Merge Two Sorted Lists.
```
```c++
class Solution {
public:
	void reorderList(ListNode* head) {
		if ( ! head ) return;
		ListNode *slow = head, *fast = head;
		while ( fast->next && fast->next->next )
		{
			slow = slow->next;
			fast = fast->next->next;
		}
			
		
		ListNode *prev = NULL, *cur = slow->next, *save;
		while ( cur )
		{
			save = cur->next;
			cur->next = prev;
			prev = cur;
			cur = save;
		}
			
		slow->next = NULL;
		
		ListNode *head2 = prev;
		while ( head2 )
		{
			save = head->next;
			head->next = head2;
			head = head2;
			head2 = save;
		}      
	}
};
```