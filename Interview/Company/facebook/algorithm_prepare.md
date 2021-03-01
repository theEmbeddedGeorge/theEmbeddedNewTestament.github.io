## High Frequency Leetcode questions for Facebook

## Array
1. Continuous Subarray Sum          Medium 
2. Longest Continous increasing subsequence     Easy
3. Best time to buy and sell stocks     Medium
4. Best time to buy and sell stocks II      Medium
5. Best Time to Buy and Sell Stock III      Medium
6. Best time to buy and sell stock with transcation fee     Medium
7. Diagnoal Tranverse       Medium
8. Task Scheduler       Medium
9. Insert Delete GetRandom O(1)     Medium
10. Product of Array Except Self        Medium
11. Kth Smallest Element in a Sorted Matrix     Medium
12. Rotate Image        Medium
13. Maximum Swap        Medium
14. Intersection of Two Arrays      Easy
15. Intersection of Two Arrays II       Easy
16. Sparse Matrix Multiplication        Medium
17. Rectangle Overlap       Easy

## Binary Search
1. Search a 2D Matrix      Easy
2. Search a 2D Matrix II   Medium
3. First Bad Version        Medium
4. Find Minimum in Rotated Sorted Array     Medium
5. Divide Two Integers      Medium
6. Find First and Last Position of Element in Sorted Array      Medium

## Linked List
1. Reverse Linked List II       Medium
2. Merge K sorted List      Medium



## Implementation

### **Continuous Subarray Sum**

***Big O:*** O(n) speed, O(1) space
```
Tips: 

Iterate and check.
```
```c++
class Solution {
public:
    /*
     * @param A: An integer array
     * @return: A list of integers includes the index of the first number and the index of the last number
     */
    vector<int> continuousSubarraySum(vector<int> &A) {
        // write your code here
        int start = 0;
        int end = start;
        int sum = 0;
        int ans = INT_MIN;
        vector<int> ret(2, 0);

        if (A.size() < 2)
            return {0, 0};
        
        for (int i = 0; i < A.size(); i++) {
            if (sum < 0) {
                sum = A[i];
                start = end = i;
            } else {
                sum += A[i];
                end = i;
            }

            if (sum > ans) {
                ret[0] = start;
                ret[1] = end;
                ans = sum;
            }
        }

        return ret;
    }
};
```
### **Longest Continous increasing subsequence**

***Big O:*** O(n) speed, O(1) space
```
Tips: 

从左到右求出最长递增和最长递减。
```
```c++
class Solution {
public:
    /**
     * @param A an array of Integer
     * @return  an integer
     */
    int longestIncreasingContinuousSubsequence(vector<int>& A) {
        // Write your code here
        int max = 1, s = 1, l = 1;
        int len = A.size();
        if (len == 0)
            return 0;
        for (int i = 1; i < len; ++i) {
            if (A[i] > A[i-1])
                s += 1;
            else {
                if (s > max) max = s;
                s = 1;
            } 
            
            if (A[i] < A[i-1])
                l += 1;
            else {
                if (l > max) max = l;
                l = 1;
            } 
        }
        if (s > max) max = s;
        if (l > max) max = l;
        return max;
    }
};
```
### **Best time to buy and sell stock (one transaction only)**

***Big O:*** O(n) speed, O(1) space
```
Tips: 

Record the min_prices and calculate the max profit along the way
```
```c++
class Solution {
public:
    /**
     * @param prices: Given an integer array
     * @return: Maximum profit
     */
    int maxProfit(vector<int> &prices) {
        // write your code here
        int min_price = INT_MAX; 
        int profit = 0;

        for (int i = 0; i < prices.size(); i++) {
            min_price = min(prices[i], min_price);
            profit = max(prices[i] - min_price, profit);
        }

        return profit;
    }
};
```


### **Best time to buy and sell stock II**

***Big O:*** O(n) speed, O(1) space
```
Tips: 

Buy low sell High!!!! YOLO WSB!!
```
```c++
class Solution {
public:
    /**
     * @param prices: Given an integer array
     * @return: Maximum profit
     */
    int maxProfit(vector<int> &prices) {
        // write your code here
        int profit = 0;
        int min_price = INT_MAX;

        for (auto p : prices) {
            min_price = min(min_price, p);
            if (p > min_price) {
                profit += (p - min_price);
                min_price = p;
            }
        }

        return profit;
    }
};
```

### **Best time to buy and sell stock III**

***Big O:*** O(n) speed, O(1) space
```
Tips: 

```
```c++
class Solution {
public:
    /**
     * @param prices: Given an integer array
     * @return: Maximum profit
     */
    int maxProfit(vector<int> &prices) {
        if (prices.size() == 0) {
            return 0;
        }
        
        vector<int> profit(prices.size());
        int buy = 0;
        buy = prices[0];
        profit[0] = 0;
        for (int i = 1; i < prices.size(); i++) {
            profit[i] = max(profit[i - 1], prices[i] - buy);
            buy = min(buy, prices[i]);
        }
        
        int sell = prices[prices.size() - 1];
        int best = 0;
        for (int i = prices.size() - 2; i >= 0; i--) {
            best = max(best, sell - prices[i] + profit[i]);
            sell = max(sell, prices[i]);
        }
        
        return best;
    }
};
```

### **Best time to buy and sell stock with transcation fee**

***Big O:*** O(n) speed, O(1) space
```
Tips: 

```
```c++
class Solution {
public:
    /**
     * @param prices: a list of integers
     * @param fee: a integer
     * @return: return a integer
     */
    int maxProfit(vector<int> &prices, int fee) {
        // write your code here
        int cash = 0, hold = -prices[0];

        for (int i = 0; i < prices.size(); i++) {
            cash = max(cash, hold + prices[i] - fee);
            hold = max(hold, cash-prices[i]);
        }
        
        return cash;
    }
};
```
### **Diagnose Tranverse**

***Big O:*** O(n*m) speed, O(man(n, m)) space
```
Tips: 

```
```c++
class Solution {
public:
    /**
     * @param matrix: a 2D array
     * @return: return a list of integers
     */
    vector<int> findDiagonalOrder(vector<vector<int>> &matrix) {
        int rows = matrix.size();
        if (rows == 0)
            return vector<int>();
        
        int cols = matrix[0].size();
        if (cols == 0)
            return vector<int>();
        
        int dig_size = rows + cols - 1;
        
        vector<int> ret;
        int i = 0, j = 0;
        for (int k = 1; k <= dig_size; k++) {
            vector<int> diag{};
            i = k >= rows ? rows-1 : k-1;
            j = k >= rows ? k-rows : 0;
            while(i >= 0 && j < cols) {
                diag.push_back(matrix[i][j]);
                i--;
                j++;
            }
            if (k%2 == 0)
                reverse(diag.begin(), diag.end());
            ret.insert(ret.end(), diag.begin(), diag.end());
        }
        
        return ret;
    }
};
```

### **Task Scheduler**

***Big O:*** O(n) speed, O(1) space
```
Tips: 
The maximum number of tasks is 26. Let's allocate an array frequencies of 26 elements to keep the frequency of each task.

Iterate over the input array and store the frequency of task A at index 0, the frequency of task B at index 1, etc.

Find the maximum frequency: f_max = max(frequencies).

Find the number of tasks which have the max frequency: n_max = frequencies.count(f_max).

If the number of slots to use is defined by the most frequent task, it's equal to (f_max - 1) * (n + 1) + n_max.

Otherwise, the number of slots to use is defined by the overall number of tasks: len(tasks).

Return the maximum of these two: max(len(tasks), (f_max - 1) * (n + 1) + n_max).
```
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

### **Insert Delete GetRandom O(1)**
**

***Big O:*** O(n) speed, O(n) space
```
Tips: 

When remove, swap the value at the desired location with the last item in the vector and update the index map.
```
```c++
class RandomizedSet {
public:
    unordered_map<int, int >mp;
    vector<int>vc;
    int n;
    /** Initialize your data structure here. */
    RandomizedSet() {
        srand(time(NULL));
        n = 0;
    }
    
    /** Inserts a value to the set. Returns true if the set did not already contain the specified element. */
    bool insert(int val) {
       if (mp.find(val) == mp.end()) {
            if (n < vc.size()) {
                vc[n] = val;
            } else {
                vc.push_back(val);
            }
            mp[val] = n;
            n++;
            return true;
        }
        return false;
    }
    
    /** Removes a value from the set. Returns true if the set contained the specified element. */
    bool remove(int val) {
           
        if (mp.find(val) != mp.end()) {
            vc[mp[val]] = vc[n-1];
            mp[vc[n-1]] = mp[val];
            n--;
            mp.erase(val);
            return true;
        }
        return false;
    }
    
    /** Get a random element from the set. */
    int getRandom() {
        if (n < 1)  
            return 0;
        return vc[rand()%n];
    }
};
```

### **Product of Array Except Self**

***Big O:*** O(n) speed, O(1) space
```
Tips: 

Keep a product array. Kind like a two pointer approach.
```
```c++
vector<int> productExceptSelf(vector<int> &nums) {
    // write your code here
    int n = nums.size();
    vector<int>res = vector<int>(n, 0);
    res[0] = 1;
    for (int i = 1; i < n; i++) {
        res[i] = res[i - 1] * nums[i - 1];
    }
    int right = 1;
    for (int i = n - 1; i >= 0; i--) {
        res[i] *= right;
        right *= nums[i];
    }
    return res;
}
};
```
### **Search a 2D Matrix**

***Big O:*** O(log(m*n)) speed, O(1) space
```
Tips: 

Binary search in all elements. Calculate the matrix coordinates.
```
```c++
class Solution {
public:
    bool searchMatrix(vector<vector<int>> &matrix, int target) {
        // write your code here
        int row = matrix.size();
        int col = matrix[0].size();

        if (row == 0 && col == 0) {
            return false;
        }

        int st, end;
        st = 0;
        end = row * col-1;
        while (st <= end) {
            int mid = st + (end-st)/2;
            int x = mid/col;
            int y = mid%col;

            if (matrix[x][y] == target) {
                return true;
            } else if (matrix[x][y] < target) {
                st = mid + 1;
            } else {
                end = mid - 1;
            }
        }

        return false;
    }
};
```
### **Search a 2D Matrix II**

***Big O:*** O(n) speed, O(1) space
```
Tips: 

Start from Bottom left and search to the upper right.
```
```c++
class Solution {
public:
    int searchMatrix(vector<vector<int>> &matrix, int target) {
        // write your code here
        int occur = 0;
        int row = matrix.size();
        int col = matrix[0].size();
        
        if (row == 0)
            return occur;

        int x = row-1;
        int y = 0;

        while (x >= 0 && y < col) {
            int val = matrix[x][y];

            if (val == target) {
                occur ++;
                x --;
            } else if (val > target) {
                x--;
            } else {
                y ++;
            }
        }

        return occur;
    }
};
```
### **First Bad version**

***Big O:*** O(log(n)) speed, O(1) space
```
Tips: 

Binary search from the end. 
```
```c++
class Solution {
public:
    int findFirstBadVersion(int n) {
        // write your code here
        int st = 1;
        int end = n;

        while (st < end) {
            int mid = st + (end - st)/2;
            if (SVNRepo::isBadVersion(mid))
                end = mid;
            else {
                st = mid + 1;
            }
        }

        return end;
    }
};
```
### **Find Minimum in Rotated Sorted Array**

***Big O:*** O(log(n)) speed, O(1) space
```
Tips: 

Binary Search
```

```c++
class Solution {
public:
    int findMin(vector<int> &nums) {
        // write your code here
        int st = 0;
        int end = nums.size()-1;

        if (nums.size() < 1)
            return -1;

        while (st < end) {
            int mid = st + (end - st)/2;

            if (nums[end] > nums[st]) {
                end = mid;
            } else {
                if (nums[mid] < nums[st])
                    end = mid;
                else 
                    st = mid + 1;
            }
        }

        return nums[end];
    }
};
```
### **Divide Two Integers**

***Big O:*** O(log(n)) speed, O(1) space
```
Tips: 

Binary Search but be careful of overflow and special cases.
```
```c++
class Solution {
public:
    int divide(int dividend, int divisor) {
        // write your code here
        if (dividend == INT_MIN && divisor == -1)
            return 2147483647;
        if (abs(divisor) == 1)
            return divisor == 1 ? dividend : -dividend;

        long dividend_long = dividend;
        long divisor_long = divisor;
        if (dividend_long == 0 || (abs(dividend_long) < abs(divisor_long)))
            return 0;

        int sign = (dividend_long < 0 && divisor_long > 0) || (dividend_long > 0 && divisor_long < 0);
        dividend_long = abs(dividend_long);
        divisor_long = abs(divisor_long);

        int quo = 0;
        int mul = 0;
        while (dividend_long > 0) {
            long divi = divisor_long << mul;
            if (dividend_long >= divi) {
                dividend_long -= divi;
                quo += 1<<mul;
                mul++;
                if (dividend_long < divisor_long)
                    break;
            } else {
                mul = 0;
            }
        }

        return sign ? -quo : quo;
    }
};
```
### **Kth Smallest Element in a Sorted Matrix**

***Big O:*** O(n*n) speed, O(1) space
```
Tips: 

Min head priority queue solution.
```
```c++
class Solution {
public:
    int kthSmallest(vector<vector<int>>& matrix, int k) {
        priority_queue<int> mxheap;
        for(auto x:matrix)
        {
            for(auto y:x)
            {
                if(mxheap.size()<k) mxheap.push(y);
                else
                {
                    if(y<mxheap.top())
                    {
                        mxheap.pop();
                        mxheap.push(y);
                    }
                }
            }
        }
        return mxheap.top();
    }
};
```

### **Find First and Last Position of Element in Sorted Array**

***Big O:*** O(log(n)) speed, O(1) space
```
Tips: 

Do binary search twice, First for head, second for the tail.
```
```c++
class Solution {
public:
    vector<int> searchRange(vector<int> &nums, int target) {
        // Write your code here.
        int st = 0;
        int end = nums.size() - 1;

        vector<int> ans = {-1, -1};
        while(st < end) {
            int mid = st + (end-st)/2;
            if (nums[mid] >= target)
                end = mid;
            else
                st = mid + 1;
        }

        if (nums[end] != target)
            return ans;
        
        ans[0] = st;

        st = 0;
        end = nums.size() - 1;
        while(st < end-1) {
            int mid = st + (end-st)/2;
            if (nums[mid] <= target)
                st = mid;
            else
                end = mid;
        }
        ans[1] = nums[end] == target ? end : st;
        
        return ans;
    }
};
```
### **Reverse Linked List II**

***Big O:*** O(n) speed, O(1) space
```
Tips: 

Reverse Linked list.
```
```c++
class Solution {
public:
    ListNode* reverseBetween(ListNode* head, int left, int right) {
        ListNode dummy;
        dummy.next = head;
        
        ListNode *pre_left;
        ListNode *left_tail;
        ListNode *tmp = &dummy, *pre, *cur;
        int count = 0;
        while(tmp) {
            if (count == left - 1) {
                pre_left = tmp;
                left_tail = tmp->next;
                cur = left_tail;
                pre = nullptr;
                tmp = tmp->next;
            } else if (count >= left && count <= right) {
                ListNode *nxt = cur->next;
                cur->next = pre;
                pre = cur;
                cur = nxt;
                if (count == right) {
                    left_tail->next = cur;
                    pre_left->next = pre;
                }
                tmp = cur;
            } else {
                tmp = tmp->next;
            }
            count ++;
        }
        
        return dummy.next;
    }
};
```

### **Merge K sorted List**

***Big O:*** O(nlog(k)) speed, O(n) space
```
Tips: 

Use a priority queue to record the smallest item in the queue. Pop it and then push the next item in the list. The comparison cost will be reduced to O(logk) for every pop and insertion to priority queue. But finding the node with the smallest value just costs O(1)O(1) time.

Approach 2: Merge with Divide And Conquer

Time complexity : O(Nlogk) where k is the number of linked lists.

We can merge two sorted linked list in O(n) time where nn is the total number of nodes in two lists.

Space complexity : O(1)

We can merge two sorted linked lists in O(1)O(1) space.
```
![Merge with DC](https://leetcode.com/problems/merge-k-sorted-lists/Figures/23/23_divide_and_conquer_new.png)
```c++
class Solution {
public:
    struct compare {
        bool operator()(ListNode* A, ListNode* B) {
            return A->val > B->val;
        }
    };

    ListNode *mergeKLists(vector<ListNode *> &lists) {
        // write your code here
        priority_queue<ListNode*, vector<ListNode*>, compare> nodes_heap;
        int n = lists.size();
        ListNode* ans, dummy;
        dummy.next = nullptr;

        for (int i = 0; i < n; i++) {
            if (lists[i] != nullptr)
                nodes_heap.push(lists[i]);
        }

        ans = &dummy;
        while(!nodes_heap.empty()) {
            ListNode* tmp = nodes_heap.top();
            nodes_heap.pop();
            ans->next = tmp;
            ans = ans->next;
            tmp = tmp->next;
            if (tmp != nullptr)
                nodes_heap.push(tmp);
        }

        return dummy.next;
    }
};

// Approach 2 (Merge Lists one by one. O(kn) speed.)
class Solution {
public:
    ListNode* merge2Lists(ListNode* l1, ListNode* l2) {
        if (!l1) return l2;
        if (!l2) return l1;
        ListNode* head = l1->val <= l2->val? l1 : l2;
        head->next = l1->val <= l2->val ? merge2Lists(l1->next, l2) : merge2Lists(l1, l2->next);
        return head;
    }
    
    ListNode* mergeKLists(vector<ListNode*>& lists) {
        if (lists.size() == 0) return NULL;
        
        ListNode* head = lists[0];
        
        for (int i = 1; i < lists.size(); i++)
            head = merge2Lists(head, lists[i]);
        
        return head;
    }
};
```

### **Ugly number**

***Big O:*** O(log(n)) speed, O(1) space
```
Tips: 
Iteration and check or recursion.
```
```c++
class Solution {
public:
    /**
     * @param num an integer
     * @return true if num is an ugly number or false
     */
    bool isUgly(int num) {
        if (num <= 0) return false;  
        if (num == 1) return true;  
          
        while (num >= 2 && num % 2 == 0) num /= 2;  
        while (num >= 3 && num % 3 == 0) num /= 3;  
        while (num >= 5 && num % 5 == 0) num /= 5;  
          
        return num == 1;  
    }
};
```

### **Rotate Image**

***Big O:*** O(M) speed, O(1) space
```
Tips: 
Let M be the number of cells in the grid.

Time complexity : O(M). We perform two steps; transposing the matrix, and then reversing each row. Transposing the matrix has a cost of O(M) because we're moving the value of each cell once. Reversing each row also has a cost of O(M), because again we're moving the value of each cell once.

Space complexity : O(1) because we do not use any other additional data structures.
```
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
        
        // transpose 
        for (int x = 0; x < rows; x++) {
            for (int y = x + 1; y < cols; y++) {
                swap(&matrix[x][y], &matrix[y][x]);
            }
        }
        
        // reflect
        for (int x = 0; x < rows; x++) {
            int st = 0, end = cols - 1;
            while (st < end) {
                swap(&matrix[x][st++], &matrix[x][end--]);
            }
        }
    }
};
```

### **Valid Tic-Tac-Toe State**

***Big O:*** O(M) speed, O(1) space
```
Tips: 
We'll count the number of 'X's and 'O's as xCount and oCount.

We'll also use a helper function win(player) which checks if that player has won. This checks the 8 horizontal, vertical, or diagonal lines of the board for if there are three in a row equal to that player.
```
```c++
class Solution {
public:
    bool is_winner(vector<string>& board, char let) {
        for (int i = 0; i < 3; i++) {
            if (board[i][0] == let && board[i][1] == let && board[i][2] == let)
                return true;
        }
        
        for (int i = 0; i < 3; i++) {
            if (board[0][i] == let && board[1][i] == let && board[2][i] == let)
                return true;
        }
        
        if (board[0][0] == let && board[1][1] == let && board[2][2] == let)
                return true;
        if (board[0][2] == let && board[1][1] == let && board[2][0] == let)
                return true;
        
        return false;
    }
    
    bool validTicTacToe(vector<string>& board) {
        // 0 for 'X', 1 for 'O'
        int xCount = 0, oCount = 0;
        
        for (auto vec : board) {
            for (auto c : vec) {
                if (c == 'X') xCount ++;
                if (c == 'O') oCount ++;
            }
        }
        
        if (xCount == 0 && oCount == 0) return true;
        if (xCount != oCount && xCount - oCount != 1) return false;
        if (is_winner(board, 'X') && (oCount != xCount-1)) return false;
        if (is_winner(board, 'O') && (xCount != oCount)) return false;
        return true;
    }
};
```

### **Maximum Swap**

***Big O:*** O(n) speed, O(1) space
```
Tips: (Greedy)

Intuition

At each digit of the input number in order, if there is a larger digit that occurs later, we know that the best swap must occur with the digit we are currently considering.

Algorithm

We will compute last[d] = i, the index i of the last occurrence of digit d (if it exists).

Afterwards, when scanning the number from left to right, if there is a larger digit in the future, we will swap it with the largest such digit; if there are multiple such digits, we will swap it with the one that occurs the latest.
```
```c++
class Solution {
public:
    /**
     * @param num: a non-negative intege
     * @return: the maximum valued number
     */
    int maximumSwap(int num) {
        vector<int> pos(10, -1);
        string number = to_string(num);
        int n = number.size();
        for (int i = 0; i < n; i++) {
            pos[number[i] - '0'] = i;
        }
        
        for (int i = 0; i < n; i++) {
            for (char j = '9'; j > number[i]; j--) {
                if (pos[j - '0'] > i) {
                    swap(number[i], number[pos[j - '0']]);
                    return stoi(number);
                }
            }
        }
        
        return num;
    }
};
```

### **Intersection of Two Arrays**

***Big O:*** O(nlog(n) + mlog(m)) speed, O(1) space
```
Sort first O(nlogn), and then iterate.
Or we can use two hash sets.
```
```c++
class Solution {
public:
    vector<int> intersection(vector<int> &nums1, vector<int> &nums2) {
        // write your code here
        sort(nums1.begin(), nums1.end());
        sort(nums2.begin(), nums2.end());
        vector<int> ans;

        int i = 0, j = 0;
        while (i < nums1.size() && j < nums2.size()) {
            if (nums1[i] < nums2[j]) {
                i++;
            } else if (nums1[i] > nums2[j]) {
                j++;
            } else {
                ans.push_back(nums1[i]);
                i++;
                j++;
            }
        }

        return ans;
    }
};
```

### **Sparse Matrix Multiplication**

***Big O:*** O(k*n^2) speed, O(1) space
```
Use unordered hash to record non-zero item first and only operates on these none-zero entries.
```
```c++
class Solution {
public:
    vector<vector<int>> multiply(vector<vector<int>> &A, vector<vector<int>> &B) {
        // write your code here        
        vector<vector<int>> ans(A.size(), vector<int>(B[0].size(), 0));
        
        for (int i = 0; i < A.size(); i++) {
            unordered_map<int, int> umap;
            for (int j = 0; j < A[0].size(); j++) {
                umap[j] = A[i][j];
            }

            for (int k = 0; k < B[0].size(); k++) {
                int sum = 0;
                for (auto item : umap) {
                    if (B[item.first][k] != 0)
                        sum += item.second * B[item.first][k];
                }
                ans[i][k] = sum;
            }
        }

        return ans;
    }
};
```
### **Rectangle Overlap**

***Big O:*** O(1) speed, O(1) space
```
Intuition

If the rectangles overlap, they have positive area. This area must be a rectangle where both dimensions are positive, since the boundaries of the intersection are axis aligned.

Thus, we can reduce the problem to the one-dimensional problem of determining whether two line segments overlap.

Algorithm

Say the area of the intersection is width * height, where width is the intersection of the rectangles projected onto the x-axis, and height is the same for the y-axis. We want both quantities to be positive.

The width is positive when min(rec1[2], rec2[2]) > max(rec1[0], rec2[0]), that is when the smaller of (the largest x-coordinates) is larger than the larger of (the smallest x-coordinates). The height is similar.
```

```c++
class Solution {
public:
    /**
     * @param l1: top-left coordinate of first rectangle
     * @param r1: bottom-right coordinate of first rectangle
     * @param l2: top-left coordinate of second rectangle
     * @param r2: bottom-right coordinate of second rectangle
     * @return: true if they are overlap or false
     */
    bool doOverlap(Point &l1, Point &r1, Point &l2, Point &r2) {
        return (min(r1.x, r2.x) > max(l1.x, l2.x)) && (min(l1.y, l2.y) > max(r1.y, r2.y));
    }
};
```