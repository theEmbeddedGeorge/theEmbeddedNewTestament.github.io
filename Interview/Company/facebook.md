## Facebook Phone Screen Preparation
### Leetcode/Lintcode Algorithm Questions List

***Linked List:***
1. Remove Nth Node From End of List            v
2. Plus One Linked List                        v
3. Intersection of Two Linked Lists            v
4. Linked List Cycle                           v
5. Linked List Cycle II                        v
6. Remove Linked List Elements                 v
7. Reverse Linked List                         v
8. Reverse Linked List II                      v
9. Delete Node in a Linked List                v
10. Delete N Nodes After M Nodes of a Linked List   v
11. Merge Two Sorted Lists                     v
12. Remove Duplicates from Sorted List II      v
13. Rotate List                                v
14. Insert into a Sorted Circular Linked List    
15. Odd Even Linked List                       v
16. Copy List with Random Pointer
17. Swap Nodes in Pairs                        v
18. Add Two Numbers                            v
19. Add Two Numbers II                         v
20. Linked List Insetion Sort                  v

***Bit Manipulation:***
1. Reverse Bits                                v 
2. Hamming Distance                            v
3. Single Number                               v
4. Single Number II    
5. Majority Element                            v
6. Range bitwise AND                           v
7. UTF-8 Validation    
8. Subsets  
9. Majority Element    
10. Maximum Length of a Concatenated String with Unique Characters    
11. Sum of Two Integers    
12. Missing Number    
13. Repeated DNA Sequences 
14. Maximum XOR of Two Numbers in an Array  
15. Counting Bits   
16. Maximum Number of Occurrences of a Substring  

***Array:***
1. Remove Duplicates from Sorted Array         v
2. Find First and Last Position of Element in Sorted Array
3. Find the Duplicate Number                   v
4. Remove Element                              v         
5. Product of array Except Self                v
6. Merge Sorted Array                          v
7. Subarray Sum Equals K

***Math:***
1.  Add Binary                                 v
2.  Plus One                                   v           
3.  Add strings                                v   
4.  Fibonacci Number                           v
5.  Pow(x, n)    
     
### Impplementations
Merge Sorted Array
```c++
class Solution {
    public void mergeSortedArray(int[] A, int m, int[] B, int n) {
        int i = m-1, j = n-1, index = m + n - 1;
        while (i >= 0 && j >= 0) {
            if (A[i] > B[j]) {
                A[index--] = A[i--];
            } else {
                A[index--] = B[j--];
            }
        }
        while (i >= 0) {
            A[index--] = A[i--];
        }
        while (j >= 0) {
            A[index--] = B[j--];
        }
    }
}
```
Product of array Except Self
```c++
class Solution {
public:
    vector<int> productExceptSelf(vector<int>& nums) {
        // setting up the necessary variables
        int len = nums.size(), prod = nums[len - 1];
        vector<int> res(len);
        // ruling out an edge case
        if (!len) return res;
        // initialising res and using it for the first pass
        res[0] = 1;
        // each cell will be the product of the previous and the matching previous value in nums
        for (int i = 1; i < len; i++) res[i] = res[i - 1] * nums[i - 1];
        // second pass, from the right
        for (int i = --len - 1; i >= 0; --i) {
            res[i] *= prod;
            prod *= nums[i];
        }
        return res;
    }
};
```
Range bitwise and
```c++
int rangeBitwiseAnd(int m, int n) {
        int curr = m & n;
        int diff = n - m;
        if (!diff) {
            return curr;
        }
        int mask = 1;
        while (diff > 1) {
            diff >>= 1;
            mask |= 1;
            mask <<= 1; 
        }
        mask |= 1;
        return curr & (~mask);
    }
```
Add Two Numbers
```c++
class Solution {
public:
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
        ListNode *tail, head;
        int sum{0}, carry{0}, first{0}, second{0};
        
        if (!l1 && !l2)
            return nullptr;
        
        tail = &head;
        while (l1 || l2 || carry) {
            first = l1 ? l1->val : 0;
            second = l2 ? l2->val : 0;
            
            sum = first + second + carry;
            carry = sum/10;
            
            tail->next = new ListNode(sum%10, nullptr);
            tail = tail->next;
            
            l1 = l1 ? l1->next : nullptr;
            l2 = l2 ? l2->next : nullptr;
        }
        
        return head.next;
    }
};
```
Swap Nodes in Pairs
```c++
class Solution {
public:
    /**
    * @param head: a ListNode
    * @return: a ListNode
    */
    ListNode * swapPairs(ListNode * head) {
        // write your code here
        ListNode *node;
        int tmp;
        
        if (!head || !head->next)
            return head;
        
        node = head;
        
        while (node && node->next) {
            tmp = node->next->val;
            node->next->val = node->val;
            node->val = tmp;
            node = node->next->next;
        }
        
        return head;
    }
};
```
Merge Sorted linked lists
```c++
class Solution {
public:
    ListNode * mergeTwoLists(ListNode * l1, ListNode * l2) {
        // write your code here
        if (!l1)
            return l2; 
        
        if (!l2)
            return l1;
            
        ListNode *head, *dummy;
        
        head = new ListNode(0);
        dummy = head;
        
        while (l1 && l2) {
            if (l1->val < l2->val) {
                dummy->next = l1;
                l1 = l1->next; 
            } else {
                dummy->next = l2;
                l2 = l2->next; 
            }
            dummy = dummy->next;
        }
        dummy->next = (l1) ? l1 : l2;
        
        return head->next;
    }
};
```
Linked List Insetion Sort 
```c++
class Solution {
public:
    ListNode* insertionSortList(ListNode* head) {
        if (!head) return NULL;
        ListNode *tail = head, *curr, *iter;
        while (tail->next) {
            // checking if the next one is already the next bigger
            if (tail->val <= tail->next->val) {
                tail = tail->next;
                continue;
            }
            // taking a new node to parse, curr, out of the list
            curr = tail->next;
            tail->next = curr->next;
            // checking if curr will become the new head
            if (curr->val < head->val) {
                curr->next = head;
                head = curr;
                continue;
            }
            // all the other cases
            iter = head;
            while (iter != tail) {
                // checking when we can splice curr between iter and the following value
                if (curr->val < iter->next->val) {
                    curr->next = iter->next;
                    iter->next = curr;
                    break;
                }
                // moving to the next!
                iter = iter->next;
            }
        }
        return head;
    }
};
```
Fibonacci Number
```c++
//recursion [Runtime: 12 ms, faster than 27.75% ]
class Solution {
public:
    int fib(int n) {
         if (n <= 1) 
            return n; 
         return fib(n-1) + fib(n-2); 
    }
};
//memoization 
class Solution {
public:
    int fib(int n,int dp[]) {
        if (n <= 1) 
           return n; 
        
        if (dp[n] != -1) 
           return dp[n];
        
        return dp[n]=fib(n-1) + fib(n-2);
    }
    int fib(int n) {
        int dp[31];
        memset(dp, -1, sizeof(dp)); 
        return fib(n,dp);
    }
};
```
Odd Even Linked List
```c++
class Solution {
public:
    ListNode* oddEvenList(ListNode* head) {
        ListNode *odd, *even, *pre, *cur, *nxt;
        
        if (!head || !head->next || !head->next->next)
            return head;
        
        odd = head; 
        even = head->next;
        
        pre = odd;
        cur = even;
        nxt = cur->next;
        
        int count = 0;
        while(nxt) {
            count ++;
            pre->next = nxt;
            
            pre = cur;
            cur = nxt;
            nxt = nxt->next;
        }
        cout << pre->val << endl;
        if (count%2 == 0) {
            pre->next = even;
            cur->next = nullptr;
        } else {
            cur->next = even;
            pre->next = nullptr;
        }
        
        return odd;
    }
};
```
Add Two Numbers II 
```c++
class Solution {
public:
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
        string a, b;
        ListNode *result = nullptr;
        while(l1) { a.push_back(l1->val+'0'); l1 = l1->next;}
        while(l2) { b.push_back(l2->val+'0'); l2 = l2->next;}
        int l = a.size()-1, r = b.size()-1, carry = 0;
        while(l >= 0 || r >= 0 || carry == 1) {
            int c = (l >= 0 ? a[l--]-'0' : 0) + ( r >= 0 ? b[r--]-'0' : 0) + carry;
            ListNode *temp = new ListNode(c%10);
            temp->next = result;
            result = temp;
            carry = c/10;
        }        
        return result;
    }
};
```
Remove Nth Node From End of List
```c++
class Solution {
public:
    ListNode* removeNthFromEnd(ListNode* head, int n) {
        if (!head) return nullptr;
        
        ListNode dummy;
        dummy.next = head;
        
        ListNode *cur = &dummy;
        while(n--) {
            cur = cur->next;
        }
        ListNode *tmp = &dummy, *pre;
        
        pre = nullptr;
        while(cur) {
            pre = tmp;
            tmp = tmp->next;
            cur = cur->next;
        }
        
        pre->next = pre->next->next;
        
        return dummy.next;
    }
};
```
Find the Duplicate Number
```c++
class Solution
{
public:
    int findDuplicate(vector<int> &nums)
    {
        int slow = nums[0];
        int fast = nums[nums[0]];

        while (fast != slow)
        {
            slow = nums[slow];
            fast = nums[nums[fast]];
        }

        fast = 0;
        while (slow != fast)
        {
            slow = nums[slow];
            fast = nums[fast];
        }

        //slow or fast doesn't matter here
        return slow;
    }
};
```
Remove Duplicate from Sorted Array
```c++
class Solution {
public:
    int removeDuplicates(vector<int>& nums) {
        int none_dup = 0;
        
        if (nums.size() < 2)
            return nums.size();
        
        for (int i = 1; i < nums.size(); i++) {
            if (nums[none_dup] != nums[i]) {
                none_dup ++;
                nums[none_dup] = nums[i];
            }
        }
        
        return none_dup + 1;
    }
};
``` 
Remove Elements
```c++
class Solution {
public:
    int removeElement(vector<int>& nums, int val) {
        int none_val = 0;
        
        for (int i = 0; i < nums.size(); i++) {
            if (nums[i] != val) {
                int tmp = nums[i];
                nums[i] = nums[none_val];
                nums[none_val] = tmp;
                none_val ++;
            }
        }
        
        return none_val;
    }
};
```
Majority Number
```c++
class Solution {
public:
    int majorityElement(vector<int>& nums) {
        int res = 0;

        for (int i = 0; i < 32; ++i) {
            int counter = 0;
            for (int j = 0; j < nums.size(); ++j)
                if (((nums[j] >> i) & 1) == 1)
                    ++counter;
    
            if (counter > nums.size()/2)
                res ^= (1 << i);
        }
    
        return res;
    }
};
```
Hamming distance
```c++
class Solution {
public:
    int hammingDistance(int x, int y) {
        int n = x ^ y;
        int len{};
        
        while (n) {
            len ++;
            n &= (n-1);
        }
        
        return len;
    }
};
```
Reverse Bits
```c++
class Solution {
public:
    uint32_t reverseBits(uint32_t n) {
        n = (n << 16) | (n >> 16);
        n = ((n << 8) & 0xff00ff00) | ((n >> 8) & 0x00ff00ff);
        n = ((n << 4) & 0xf0f0f0f0) | ((n >> 4) & 0x0f0f0f0f);
        n = ((n << 2) & 0xcccccccc) | ((n >> 2) & 0x33333333);
        n = ((n << 1) & 0xaaaaaaaa) | ((n >> 1) & 0x55555555);
        
        return n;
    }
};
class Solution {
public:
    uint32_t reverseBits(uint32_t n) {
        uint32_t ret{};
        int m = 32;
        
        while(m --) {
            ret <<= 1;
            ret |= (n & 0x1);
            n >>= 1;
        }
        
        return ret;
    }
};
```

Remove Linked List Elements
```c++
class Solution {
public:
    ListNode* removeElements(ListNode* head, int val) {
        ListNode dummy, *pre, *cur;
        
        if (!head)
            return head;
        
        dummy.next = head;
        
        pre = &dummy;
        cur = head;
        
        while (cur) {
            if (cur->val == val) {
                cur = cur->next;
            } else {
                pre->next = cur;
                cur = cur->next;
                pre = pre->next;
            }
        }
        pre->next = cur;
        
        return dummy.next;
    }
};
```
Intersection of Two Linked Lists
```c++
class Solution {
public:
    ListNode *getIntersectionNode(ListNode *headA, ListNode *headB) {
        ListNode *A, *B;
        int lenA{}, lenB{}, diff{};
        
        if (!headA || !headB)
            return nullptr;
        
        A = headA;
        B = headB;
        
        while(A) {
            lenA++;
            A = A->next;
        }
        
        while(B) {
            lenB++;
            B = B->next;
        }
        
        diff = abs(lenA-lenB);
        ListNode *tmp = lenA > lenB ? headA : headB;
        
        while(diff--) tmp = tmp->next;
        
        headA = lenA > lenB ? tmp : headA;
        headB = lenB > lenA ? tmp : headB;
        
        while(headA && headB) {
            if (headA == headB) return headA;
            headA = headA->next;
            headB = headB->next;
        }
        
        return nullptr;
    }
};
```
Delete N Nodes After M Nodes of a Linked List
```c++
class Solution {
public:
    ListNode* deleteNodes(ListNode* head, int m, int n) {
        if (!head || !head->next) return nullptr;
        
        ListNode dummy;
        dummy.next = head;
        
        ListNode *pre = &dummy;
        
        while (pre) {
            int x = m;
            while (x-- && pre) {
                pre = pre->next;
            }
            if (!pre) break;
            ListNode *cur = pre->next;
            int y = n;
            while (y-- && cur) {
                cur = cur->next;
            }
            pre->next = cur;
        }
        
        return dummy.next;
    }
};
```
Delete a Node in a linked list (not tail)
```c++
class Solution {
public:
    void deleteNode(ListNode* node) {
        if (!node) return;
        
        *node = *(node->next);
    }
};
```

Linked List Cycle II
```c++
class Solution {
public:
    ListNode *detectCycle(ListNode *head) {
        if (!head || !head->next)
            return nullptr;
        
        ListNode *slow, *fast;
        slow = fast = head;
        
        while(fast && fast->next) {
            slow = slow->next;
            fast = fast->next->next;
            if (slow == fast) {
                slow = head;
                while(fast != slow) {
                    slow = slow->next;
                    fast = fast->next;
                }
                return slow;
            }
        }
        
        return nullptr;
    }
};
```

Linked List Cycle
```c++
bool hasCycle(struct ListNode *head) {
    if (!head || !head->next)
        return false;
    
    struct ListNode *slow, *fast;
    
    slow = fast = head;
    
    while(fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
        
        if (slow == fast) return true;
    }
    
    return false;
}
```
Reverse Linked List II
```c++
/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode() : val(0), next(nullptr) {}
 *     ListNode(int x) : val(x), next(nullptr) {}
 *     ListNode(int x, ListNode *next) : val(x), next(next) {}
 * };
 */

class Solution {
public:
   ListNode* reverseNode(ListNode* head, ListNode* target) {
        ListNode *pre, *cur, *nxt;
        
        pre = nullptr;
        cur = head;
        
        while(cur != target) {
            nxt = cur->next;
            
            cur->next = pre;
            pre = cur;
            cur = nxt;
        }
        
        head->next = target;
        return pre;
    }
    
    ListNode* reverseBetween(ListNode* head, int m, int n) {
        ListNode dummy;
        dummy.next = head;
        
        if (!head)
            return head;
        
        ListNode *pre, *cur, *target;
        
        pre = &dummy;
        cur = head;
        target = head;
        while(--m) {
            pre = cur;
            cur = cur->next;
        }
        
        while(n--) {
            target = target->next;
        }
        
        pre->next = reverseNode(cur, target);
        
        return dummy.next;
    }
};
```

Add strings
```c++
class Solution {
public:
    string addStrings(string num1, string num2) {
        int n = num1.size() - 1;
        int m = num2.size() - 1;
        string ans{};

        int carry = 0;
        while (n >= 0 || m >=0 || carry) {
            int sum = carry;
            if (n >= 0) sum += num1[n] - '0';
            if (m >= 0) sum += num2[m] - '0';
            
            ans += sum%10 + '0';
            carry = sum/10;
            n--; m--;
        }
        
        reverse(ans.begin(), ans.end());
        return ans;
    }
};
```

Add addBinary
```c++
class Solution {
public:
    string addBinary(string a, string b) {
        string ans="";
        int i=a.length()-1;
        int j=b.length()-1;
        int carry=0;
        
        while(i>=0 || j>=0)
        {
            int temp=carry;
            if(i>=0) temp+=(a[i]-'0'); // converting string into integer
            if(j>=0) temp+=(b[j]-'0');
            ans+=(temp%2+'0'); // if both digit is 1 then append 0
            carry=temp/2; // if sum is 2 then carry 1
            i--,j--;
        }
        if(carry!=0) ans+=(carry%2+'0');
        reverse(ans.begin(),ans.end());
        return ans;
    }
};
```

Reverse Linked List
```c++
class Solution {
public:
    ListNode* reverseList(ListNode* head) {
        if (!head || !head->next)
            return head;
        
        ListNode *pre, *cur, *nxt;
        pre = nullptr;
        cur = head;
        
        while (cur) {
            nxt = cur->next;
            
            cur->next = pre;
            pre = cur;
            cur = nxt;
        } 
        
        return pre;
    }
};
```

Plus One
```c++
class Solution {
public:
    vector<int> plusOne(vector<int>& digits) {
        int n = digits.size()-1;
        
        int carry = 1;
        for (int i = n; i >= 0; i--) {
            digits[i] = digits[i] + carry;
            
            if (digits[i] < 10)
                return digits;
            else {
                digits[i] = digits[i]%10;
            }
        }
        
        digits.push_back(0);
        digits[0] = 1;
        
        return digits;
    }
};
```


Remove Duplicates from Sorted List II 
```c++
class Solution {
public:
    ListNode* deleteDuplicates(ListNode* head) {
        ListNode dummy;
        dummy.next = head;
        
        ListNode *pre, *cur, *nxt;
        pre = &dummy;
        cur = head;
        
        int flag = 0;
        while (cur && cur->next) {
            nxt = cur->next;

            if (nxt->val == cur->val) {
                cur = cur->next;
                flag = 1;
            } else {
                if (flag) {
                    pre->next = nxt;
                    cur = nxt;
                    flag = 0;
                } else {
                    cur = nxt;
                    pre = pre->next;
                }
            }
        }
        if (flag) pre->next = nullptr;
        else pre->next = cur;
        
        return dummy.next;
    }
};
```  

Rotate List
```c++
class Solution {
public:
    ListNode* rotateRight(ListNode* head, int k) {
        
        if (!head || !head->next || !k)
            return head;
        
        ListNode *tail = head;
        int len = 1;
        while(tail->next) {
            len ++;
            tail = tail->next;
        }
        
        int K = k%len;
        if (K == 0) return head;
        
        tail->next = head;
        
        int move = len - K;
        
        ListNode *pre, *cur;
        pre = nullptr;
        cur = head;
        while (move--) {
            pre = cur;
            cur = cur->next;
        }
        pre -> next = NULL;
        
        return cur;
    }
};
```