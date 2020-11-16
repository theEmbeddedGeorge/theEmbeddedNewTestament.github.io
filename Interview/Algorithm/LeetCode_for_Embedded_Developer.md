## LeetCode Questions For Embedded Developers

### Question Selection Criteria

#### **No advanced data structures (ex. priority queue) required. High frequency questions focusing on C programming.**:yum:

LeetCode # | Title | Diffculty 
----------------|-------|----------
1 |	Two Sum | Easy	
14 | Longest Common Prefix | Easy	
19 | Remove Nth Node From End of List | Medium	
20 | Valid Parentheses | Easy	
21 | Merge Two Sorted Lists | Easy	
24 | Swap Nodes in Pairs | Medium	
26 | Remove Duplicates from Sorted Array | Easy	
27 | Remove Element | Easy	
28 | Implement strStr() | Easy	
33 | Search in Rotated Sorted Array | Medium	
38 | Count and Say | Easy	
53 | Maximum Subarray | Easy	
58 | Length of Last Word | Easy	
66 | Plus One | Easy	
67 | Add Binary | Easy	
83 | Remove Duplicates from Sorted List | Easy	
88 | Merge Sorted Array | Easy	
100 | Same Tree | Easy	
104 | Maximum Depth of Binary Tree | Easy	
108 | Convert Sorted Array to Binary Search Tree | Easy	
109 | Convert Sorted List to Binary Search Tree | Medium	
110 | Balanced Binary Tree | Easy	
111 | Minimum Depth of Binary Tree | Easy	
112 | Path Sum | Easy	
114 | Flatten Binary Tree to Linked List | Medium	
121 | Best Time to Buy and Sell Stock | Easy	
125 | Valid Palindrome | Easy	
136 | Single Number | Easy	
137 | Single Number II | Medium	
141 | Linked List Cycle | Easy	
160 | Intersection of Two Linked Lists | Easy	
169 | Majority Element | Easy	
190 | Reverse Bits | Easy	
191 | Number of 1 Bits | Easy	
203 | Remove Linked List Elements | Easy	
206 | Reverse Linked List | Easy	
231 | Power of Two | Easy	
234 | Palindrome Linked List | Easy	
237 | Delete Node in a Linked List | Easy	
260 | Single Number III | Medium	
268 | Missing Number | Easy	
274 | H-Index | Medium	
278 | First Bad Version | Easy	
279 | Perfect Squares | Medium	
283 | Move Zeroes | Easy	
292 | Nim Game | Easy	
299 | Bulls and Cows | Medium	
300 | Longest Increasing Subsequence | Medium	
313 | Super Ugly Number | Medium	
318 | Maximum Product of Word Lengths | Medium	

### Longest Common Prefix

    class Solution {
    public:
        /**
        * @param strs: A list of strings
        * @return: The longest common prefix
        */
        string longestCommonPrefix(vector<string> &strs) {
            // write your code here
            string ret = "";
            
            if (strs.size() == 0)
                return ret;
                
            for (int i = 0; i < strs[0].size(); i ++)  {
                char c = strs[0][i];
                
                for (auto s : strs) {
                    if (i >= s.size() || c != s[i])
                        return ret;
                }
                
                ret += c;
            }
            
            return ret; 
        }
    };

### Remove Nth Node From End of List

    class Solution {
    public:
        /**
        * @param head: The first node of linked list.
        * @param n: An integer
        * @return: The head of linked list.
        */
        ListNode * removeNthFromEnd(ListNode * head, int n) {
            // write your code here
            
            ListNode *fast, *slow;
            
            if (!head || n == 0) return head;
            
            if (!head->next && n) return NULL;
            
            slow = fast = head;
            
            while(n-- && fast) fast = fast->next;
            
            if (!fast)
                return slow->next;
            
            fast = fast->next;
            while (fast) {
                fast = fast->next;
                slow = slow->next;
            }
            
            
            slow->next = slow->next->next;
            
            return head;
        }
    };

### Valid Parentheses
    class Solution {
    public:
        /**
        * @param s: A string
        * @return: whether the string is a valid parentheses
        */
        bool isValidParentheses(string &s) {
            // write your code here
            vector <char> stac;
            
            for (int i = 0; i < s.size(); i ++) {
                switch (s[i]) {
                    case '(':
                    case '[':
                    case '{':
                        stac.push_back(s[i]);
                        break;
                    case ')':
                    case ']':
                    case '}':
                        if (stac.size() == 0)
                            return false;
                        char ch = stac.back();
                        if (s[i] == ')' && ch != '(')
                            return false;
                        if (s[i] == ']' && ch != '[')
                            return false;
                        if (s[i] == '}' && ch != '{')
                            return false;
                        stac.pop_back();
                        break;
                }
            }
            
            if (stac.size() == 0)
                return true;
            
            return false;
        }
    };

### Merge Two Sorted Lists
    class Solution {
    public:
        /**
        * @param l1: ListNode l1 is the head of the linked list
        * @param l2: ListNode l2 is the head of the linked list
        * @return: ListNode head of linked list
        */
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

### Swap Nodes in Pairs
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

### Remove Duplicates from Sorted Array
    class Solution {
    public:
        /*
        * @param nums: An ineger array
        * @return: An integer
        */
        int removeDuplicates(vector<int> &nums) {
            // write your code here
            int nextNoDup = 1;
            
            if (!nums.size())
                return 0;
            
            for (int i = 0; i < nums.size(); i ++) {
                if (nums[nextNoDup-1] != nums[i]) {
                    nums[nextNoDup] = nums[i];
                    nextNoDup ++;
                }
            }
            
            return nextNoDup;
        }
    };

### Remove Element
    class Solution {
    public:
        /*
        * @param A: A list of integers
        * @param elem: An integer
        * @return: The new length after remove
        */
        int removeElement(vector<int> &A, int elem) {
            // write your code here
            int i, noElement = 0;
            
            for (i = 0; i < A.size(); i++) {
                if (A[i] != elem) {
                    A[noElement++] = A[i];
                }  
            }
            
            return noElement;
        }
    };

### Implement StrStr()
    class Solution {
    public:
        /**
        * @param source: 
        * @param target: 
        * @return: return the index
        */
        int strStr(string &source, string &target) {
            // Write your code here
            int ret = 0;
            
            if (source.size() < target.size())
                return -1;
                
            if (target.size() == 0)
                return 0;
            
            for (ret; ret < source.size()-target.size() + 1; ret++) {
                if (source[ret] == target[0]) {
                    for (int i = 0; i < target.size(); i++) {
                        if (source[ret+i] != target[i])
                            break;
                        else if (i == target.size()-1) {
                            return ret;
                        }
                    }
                }
            }
            
            return -1;
        }
    };

### Search in a rotated sorted array
    class Solution {
    public:
        /**
        * @param A: an integer rotated sorted array
        * @param target: an integer to be searched
        * @return: an integer
        */
        static int searchT(vector<int> &A, int target, int f, int e) {
            int m = (f + (e - f)/2);
            cout << m << endl;
            
            if (A[m] == target)
                return m;
            
            if (f == e)
                return -1;
                
            if (f == m)
                return (A[e] == target) ? e : -1;
            
            // Normal sorted order
            if (A[e] > A[f]) {
                return (A[m] > target) ? searchT(A, target, f, m-1) : searchT(A, target, m+1, e);
            // Pivoted order
            } else {
                if (A[m] > A[f]) {
                    return (target < A[m] && target > A[f]) ? searchT(A, target, f, m-1) : searchT(A, target, m+1, e);
                } else {
                    return (target > A[m] && target < A[e]) ? searchT(A, target, m+1, e) : searchT(A, target, f, m-1);
                }
            }
        } 
        
        int search(vector<int> &A, int target) {
            // write your code here
            if (A.size() == 0)
                return -1;
            
            return searchT(A, target, 0, A.size()-1);
        }
    };

### Count and Say
    class Solution {
    public:
        /**
        * @param n: the nth
        * @return: the nth sequence
        */
        
        static string countSay (string str) {
            int count, i;
            string new_str = "";
            
            count = 1;
            for (i = 0; i < str.size()-1; i ++) {
                if (str[i+1] != str[i]) {
                    new_str += ('0'+count);
                    new_str += (str[i]);
                    count = 1;
                }
                else
                    count ++;
            }
            // handle the last element
            new_str += ('0'+count);
            new_str += (str[i]);
            
            return new_str;
        }
        
        string countAndSay(int n) {
            // write your code here
            string ret_str = "1";
            
            if (n == 0)
                return "";
                
            while (--n)
                ret_str = countSay(ret_str);
            
            return ret_str;
        }
    };

### Maximum Subarray
    class Solution {
    public:
        /**
        * @param nums: A list of integers
        * @return: An integer indicate the sum of max subarray
        */
        int maxSubArray(vector<int> nums) {
            int sum = 0, minSum = 0, maxSum = INT_MIN;
            for (int i = 0; i < nums.size(); i++) {
                sum += nums[i];
                maxSum = max(maxSum, sum - minSum);
                minSum = min(minSum, sum);
            }
            return maxSum;
        }
    };

### Length of last word
    class Solution {
    public:
        /**
        * @param s: A string
        * @return: the length of last word
        */
        int lengthOfLastWord(string &s) {
            // write your code here
            int i, j;
            
            if (s.size() == 0)
                return 0;
            
            i = s.size() - 1;
            while (s[i] == ' ') i--;
            
            for (j = i; j >= 0; j --) {
                if (s[j] == ' ')
                    return i - j;
            }
            
            if (j < 0)
                return i+1;
            
            return 0;
        }
    };

### Plus One
    class Solution {
    public:
        vector<int> plusOne(vector<int> &digits) {
            add(digits, 1);
            return digits;
        }
    private:
        // 0 <= digit <= 9
        void add(vector<int> &digits, int digit) {
            int c = digit; // carry, 进位
            for (auto it = digits.rbegin(); it != digits.rend(); ++it) {
                *it += c;
                c = *it / 10;
                *it %= 10;
            }
            if (c > 0) digits.insert(digits.begin(), 1);
        }
    };

### Remove Duplicates from Sorted List
    class Solution {
    public:
        /**
        * @param head: head is the head of the linked list
        * @return: head of linked list
        */
        ListNode * deleteDuplicates(ListNode * head) {
            ListNode *dummy, *pre, *cur; 
            
            // write your code here
            if (!head || !head->next)
                return head;
                
            dummy = pre = head;
            cur = head->next;
                
            while (cur) {
                if (pre->val != cur->val) {
                    dummy -> next = cur;
                    dummy = cur;
                }
                pre = cur;
                cur = cur ->next;
            }
            dummy->next = cur;
                
            return head;        
        }
    };

### Merge Sorted Array
    class Solution {
    public:
        /*
        * @param A: sorted integer array A which has m elements, but size of A is m+n
        * @param m: An integer
        * @param B: sorted integer array B which has n elements
        * @param n: An integer
        * @return: nothing
        */
        void mergeSortedArray(int A[], int m, int B[], int n) {
            // write your code here
            int i, j, k;
            i = m - 1;
            j = n - 1;
            k = m + n - 1;
            
            while(i >= 0 && j >= 0) {
                A[k--] = A[i] > B[j] ? A[i--] : B[j--];
            }
            
            while (i >= 0)
                A[k--] = A[i--];
                
            while (j >= 0)
                A[k--] = B[j--];
            
        }
    };

### Same Tree
    /**
    * Definition of TreeNode:
    * class TreeNode {
    * public:
    *     int val;
    *     TreeNode *left, *right;
    *     TreeNode(int val) {
    *         this->val = val;
    *         this->left = this->right = NULL;
    *     }
    * }
    */

    class Solution {
    public:
        /**
        * @param a: the root of binary tree a.
        * @param b: the root of binary tree b.
        * @return: true if they are identical, or false.
        */
        
        bool isIdentical(TreeNode * a, TreeNode * b) {
            // write your code here
            if (!a && !b)
                return true;
                
            if (!a || !b)
                return false;
            
            if (a->val == b->val)
                return isIdentical(a->left, b->left) & isIdentical(a->right, b->right);
            else
                return false;
        }
    };

### Max depth of a Tree
    /**
    * Definition of TreeNode:
    * class TreeNode {
    * public:
    *     int val;
    *     TreeNode *left, *right;
    *     TreeNode(int val) {
    *         this->val = val;
    *         this->left = this->right = NULL;
    *     }
    * }
    */

    class Solution {
        public:
            /**
            * @param root: The root of binary tree.
            * @return: An integer
            */
            int maxDepth(TreeNode * root) {
                int lft, rht;
                
                // write your code here
                if (!root)
                    return 0;
                
                lft = maxDepth(root->left);
                rht = maxDepth(root->right);
                
                return lft > rht ? lft+1 : rht+1;
            }
        };