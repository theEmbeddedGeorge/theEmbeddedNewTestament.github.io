## Advance Leetcode questions
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
21. Reorder List                               v

***Bit Manipulation:***
1. Reverse Bits                                v 
2. Hamming Distance                            v
3. Single Number                               v
4. Single Number II                            v
5. Majority Element                            v
6. Range bitwise AND                           v
7. UTF-8 Validation                            v  
8. Subsets  
9. Majority Element                            v    
10. Maximum Length of a Concatenated String with Unique Characters    
11. Sum of Two Integers    
12. Missing Number    
13. Repeated DNA Sequences                     v
14. Maximum XOR of Two Numbers in an Array  
15. Counting Bits   
16. Maximum Number of Occurrences of a Substring  
17. Number Complement                          v
18. Convert a Number to Hexadecimal            v
19. Power of Four                              v
20. XOR Operation in an Array                  v
21. XOR Queries of a Subarray                  v

***Array:***
1. Remove Duplicates from Sorted Array         v
2. Find First and Last Position of Element in Sorted Array  v
3. Find the Duplicate Number                   v
4. Remove Element                              v         
5. Product of array Except Self                v
6. Merge Sorted Array                          v
7. Subarray Sum Equals K                       v
8. Task Scheduler                              v
9. Leftmost Column with at Least a One         v
10. First Bad Version                          v
11. Count and Say                              v
12. Maximum Number of Occurrences of a Substring  v
13. Maximum Subarray                           v
14. Move Zeroes                                v
15. Find the Duplicate Number                  v
16. Container With Most Water                  v
17. Verifying an Alien Dictionary              v
18. Running Sum of 1d Array                    v
19. Continuous Subarray Sum                    v
20. Random Pick with Weight                    v
21. Friends Of Appropriate Ages                v
22. Trap Rain water                            v
23. House Robber                               v
24. Degree of an array                         v
25. Decode Ways                                v
26. Best time to sell stocks II                v
27. Monotonic Array                            v
28. Longest Continuous Increasing Subsequence  v
29. Maximum Product Subarray                   v
30. First Missing Positive                     v
31. Maximum Swap                               v
32. Meeting Room II                            v
33. Sort Colors                                v
34. 3 Sum                                      v

***Math:***
1.  Add Binary                                 v
2.  Plus One                                   v           
3.  Add strings                                v   
4.  Fibonacci Number                           v
5.  Pow(x, n)                                  v
6.  Divide Two Integers                        v
7.  Reverse Integer without long long          v
8.  Palindrome Number                          v
9.  Dot Product of Two Sparse Vectors          v
10. Ugly Number                                v

***Matirx:***
1.  Give the center of a matrix and then draw circle
2.  Rotate a matrix by 90 degree               v
3.  Sparse Matrix Multiplication
4.  Search 2D Matrix
5.  Diagonal Traverse                          v

***Data Structure:***
1. Insert Delete GetRandom O(1)                v
2. LRU Cache
3. Design Add and Search Words Data Structure
4. Find Median from Data Stream                v

***String:***
1. Valid Palindrome II                         v
2. Valid Palindrome                            v
3. Decode String                               v
4. Find All Anagrams in a String               v
     
### Impplementations
Verifying an Alien Dictionary
```c++
class Solution {
public:
    bool isAlienSorted(vector<string>& words, string order) {
        for (int i = 0; i < words.size() - 1; i++) {
        string word1 = words[i];
        string word2 = words[i + 1];
        int i1 = 0, i2 = 0;
        while (word1[i1] == word2[i2]) {
            i1++, i2++;
        }
        int r = order.find(word1[i1]);   
        int s = order.find(word2[i2]);
        if (r > s) return false;
    }
    return true;
    }
};
```
Container With Most Water
```c++
class Solution {
public:
    int maxArea(vector<int>& height) {
        int max_a = 0; 
        
        int st = 0, end = height.size()-1;
        while (st < end) {
            int water = min(height[st], height[end]) * (end-st);
            max_a = max(max_a, water);
            if (height[st] < height[end])
                st ++;
            else
                end --;
        }
        
        return max_a;
    }
};
```
Find the Duplicate Number
```c++
class Solution {
public:
    int findDuplicate(vector<int>& nums) {
        int slow, fast;
        slow = fast = 0;
        
        do {
            slow = nums[slow];
            fast = nums[nums[fast]];
        }
        while (slow != fast);
        
        slow = 0;
        while (slow != fast) {
            slow = nums[slow];
            fast = nums[fast];
        }
        
        return fast;
    }
};
```
Rotate a matrix by 90 degree
```c++
class Solution {
public:
    
    void swap(int *a, int *b) {
        int temp = *a;
        *a = *b;
        *b = temp;
    }
    
    void rotate(vector<vector<int>>& matrix) {
        int rows = matrix.size();
        int cols = matrix[0].size();
        
        for (int x = 0; x < rows; x++) {
            for (int y = x + 1; y < cols; y++) {
                swap(&matrix[x][y], &matrix[y][x]);
            }
        }
    
        for (int x = 0; x < rows; x++) {
            int st = 0, end = cols - 1;
            while (st < end) {
                swap(&matrix[x][st++], &matrix[x][end--]);
            }
        }
    }
};
```
Divide Two Integers
```c++
class Solution {
public:
    int divide(int dividend, int divisor) {
        long long n=dividend, m=divisor;
        if(n <= INT_MIN && m == -1) return INT_MAX;

        int sign = (n < 0) ^ (m < 0) ? -1: 1;

        n=abs(n);
        m=abs(m);

        long long q=0, temp=0;

        for(int i=31; i>=0; i--){
            if(temp + (m << i) <= n){
                temp += m << i;
					q += 1 << i;      //  q |= 1 << i;
            }
        }
        return sign * q;
    }
};
```
Move Zeroes
```c++
class Solution {
public:
    void moveZeroes(vector<int>& nums) {
        int lastNonZeroFoundAt = 0;
        // If the current element is not 0, then we need to
        // append it just in front of last non 0 element we found. 
        for (int i = 0; i < nums.size(); i++) {
            if (nums[i] != 0) {
                nums[lastNonZeroFoundAt++] = nums[i];
            }
        }
        // After we have finished processing new elements,
        // all the non-zero elements are already at beginning of array.
        // We just need to fill remaining array with 0's.
        for (int i = lastNonZeroFoundAt; i < nums.size(); i++) {
            nums[i] = 0;
        }
    }
};
```
Maximum Subarray
```c++
class Solution {
public:
    int maxSubArray(vector<int>& nums) {
        int n = nums.size();
        int sum = nums[0];
        int max_v = sum;
        
        for (int i = 1; i < n; i++) {
            sum = max(nums[i], sum+nums[i]);
            max_v = max(sum, max_v);
        }
        
        return max_v;  
    }
};
```
Maximum Number of Occurrences of a Substring
```c++
class Solution {
    
public:
    unordered_map<string, int> occur;
    unordered_map<char, int> umap;
    
    int maxFreq(string s, int maxLetters, int minSize, int maxSize) {
        int max_val = 0;
        
        string tmp = s.substr(0, minSize);
        for (auto c : tmp)
            umap[c] ++;

        if (umap.size() <= maxLetters) {
            occur[tmp]++;
            max_val = max(max_val, occur[tmp]);
        }

        for (int end = minSize; end < s.size(); end++) {
            if (--umap[s[end-minSize]] == 0)
                umap.erase(s[end-minSize]);
            umap[s[end]] ++;
            
            if (umap.size() <= maxLetters) {
                string str_tmp = s.substr(end-minSize+1, minSize);
                occur[str_tmp]++;
                max_val = max(max_val, occur[str_tmp]);
            }
        }
        
        return max_val;
    }
};
```
XOR Queries of a Subarray
```c++
class Solution {
public:
    vector<int> xorQueries(vector<int>& arr, vector<vector<int>>& queries) {
        vector<int> res;
        
        for (int i = 1; i < arr.size(); i++) {
            arr[i] ^= arr[i-1];
        }
        
        for (auto &v : queries) {
            if (v[0] == 0)
                res.push_back(arr[v[1]]);
            else {
                res.push_back(arr[v[1]]^arr[v[0]-1]);
            }
            
        }
        
        return res;
    }
};
```
Count and Say
```c++
class Solution {
public:
    string countAndSay(int n) {
        string res = "1";
        for (int i = 2; i <= n; i++) {
            string tmp = "";
            char c = res[0];
            int count = 1;
            for (int j = 1; j < res.size(); j++) {
                if (res[j] != c) {
                    tmp.push_back('0' + count);
                    tmp.push_back(c);
                    count = 0;
                    c = res[j];
                }
                count ++;
            }
            tmp.push_back('0' + count);
            tmp.push_back(c);
            res = tmp;
        }
        
        return res;
    }
};
```
Repeated DNA Sequences
```c++
class Solution {
public:
    int get_int(string &s) {
        int ret = 0;
        for(int i = 0; i < 10; i++) {
            char c = s[i];
            int tmp;
            switch(c) {
                case 'A':
                    tmp = 0x0;
                    break;
                case 'C':
                    tmp = 0x1;
                    break;
                case 'G':
                    tmp = 0x2;
                    break;
                case 'T':
                    tmp = 0x3;
                    break;
            }
            ret <<= 2;
            ret |= tmp;
        }
        
        return ret;
    }
    
    vector<string> findRepeatedDnaSequences(string s) {
        vector<string> res;
        unordered_map<int, int> um;
        
        if (s.size() <= 10)
            return res;
        
        for (int i = 0; i < s.size() - 9; i++) {
            string tmp = s.substr(i, 10);
            int ret_int = get_int(tmp);
            um[ret_int] ++;
            if (um[ret_int] == 2)
                res.push_back(tmp);
        }
        
        return res;
    }
};
```
XOR Operation in an Array
```c++
class Solution {
public:
    int xorOperation(int n, int start) {
        int res = 0;
        
        for (int i = 0; i < n; i++) {
            res ^= start;
            start += 2;
        }
        
        return res;
    }
};
```
Power of Four
```c++
class Solution {
public:
    bool isPowerOfFour(int num) {
        return (num > 0) && ((num & (num - 1)) == 0) && ((num & 0xaaaaaaaa) == 0);
    }
};
```
Convert a Number to Hexadecimal
```c++
class Solution {
public:
    string toHex(int num) {
        if(num == 0){
            return "0";
        }
        unsigned int num1 = num; // takes care of negative integers automatically
        string hex = "";
        int base = 16;
        int rem;
        while(num1 > 0){
            rem = num1 % base;
            if(rem < 10){
                hex.insert(hex.begin(), ('0' + (rem)));
            }else{
                hex.insert(hex.begin(), ('a' + (rem%10)));
            }
            num1 = num1 / base;
        }

        return hex;
    }
};
```
Number Complement
```c++
class Solution {
public:
    int findComplement(int num) {
        int bit = 1, tp = num;
        while(tp) {
            num ^= bit;
            bit <<= 1;
            tp >>= 1;
        }
        return num;
    }
};
```
First Bad Version
```Binary Search, O(Log(n))```
```c++
class Solution {
public:
    int firstBadVersion(int n) {
        int st = 1;
        int end = n;
        
        while (st < end) {
            int mid = st + (end - st)/2;
            
            if (isBadVersion(mid)) {
                end = mid;
            } else {
                st = mid + 1;
            }
        }
        
        return end;
    }
};
```
Leftmost Column with at Least a One
```Binary Search fron the end of column, update the min column on the way. O(MLog(N))```
```c++
class Solution {
    public:
        int leftMostColumnWithOne(BinaryMatrix &binaryMatrix) {
            int rows = binaryMatrix.dimensions().front();
            int max_col = binaryMatrix.dimensions().back()-1;
            int res = -1;
            for(int i = 0; i < rows; ++i)
                if(binaryMatrix.get(i, max_col))
                {
                    int start = 0, end = max_col;
                    while(start <= end)
                    {
                        int mid = (start+end)/2;
                        binaryMatrix.get(i, mid) ? end = mid-1 : start = mid+1;
                    }
                    res = max_col = start;
                }
            return res;
        }
};
```
Single Number II
```c++
class Solution {
public:
    void count_bits(int arr[], int val) {
        for (int i = 0; i < 32; i++) {
            if (val & (0x1 << i))
                arr[i] ++;
        }
    }
    
    int singleNumber(vector<int>& nums) {
        int arr[32] = {};
        int ret = 0;
        
        for (auto num : nums) {
            count_bits(arr, num);
        }
        
        for (int i = 0; i < 32; i++) {
            arr[i] = arr[i]%3;
            ret |= (arr[i] << i);
        }
        
        return ret;
    }
};
```
Find First and Last Position of Element in Sorted Array
```c++
class Solution {
public:
    vector<int> searchRange(vector<int>& nums, int target) {
        int l = 0, r = nums.size() - 1;
        int mid;
        vector<int> ret;
        
        if (nums.size() == 0)
            return {-1, -1};
        
        while(l < r) {
            mid = l + (r-l)/2;
            
            if (nums[mid] >= target) {
                r = mid;
            } else {
                l = mid+1;
            }
        }
        
        if (nums[r] != target)
            return {-1, -1};
        else {
            ret.push_back(r);
            while(r < nums.size() && nums[r] == target) {
                r ++;
            }
            ret.push_back(r-1);
        }
        return ret;
    }
};
```
Subarray Sum Equals K
```c++
class Solution {
public:
    int subarraySum(vector<int>& nums, int k) {
        int count = 0, sum = 0;
        unordered_map<int, int> m;
        m[0] = 1;
        for (int i=0; i<nums.size(); i++) {
            sum += nums[i];
            if (m.find(sum - k) != m.end())
                count += m[sum - k];
            m[sum]++;
        }
        return count;
    }
};
```
Task Scheduler
```c++
class Solution {
public:
    int cnt[26], maxcnt = 0, e = 0;
    int leastInterval(vector<char>& tasks, int n) {
        for (char c : tasks) cnt[c-'A']++;
        for (int i = 0; i < 26; i++) maxcnt = max(maxcnt, cnt[i]);
        for (int i = 0; i < 26; i++) 
            if (cnt[i] == maxcnt) e++;
        return max(tasks.size(), (maxcnt-1)*(n+1) + e);
    }
};
```
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
