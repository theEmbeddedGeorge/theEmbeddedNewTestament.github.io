## Problems

1. Running Sum of 1d Array
2. Continuous Subarray Sum
3. Random Pick with Weight
4. Friends Of Appropriate Ages
5. Trap Rain Water
6. House Robber
7. Degree of an array
8. Decode Ways
9. Best Time to sell stocks II
10. Monotonic Array
11. Longest Continuous Increasing Subsequence
12. Maximum Product Subarray
13. First Missing Positive
14. Maximum Swap
15. Meeting Room II
16. Sort Colors
17. 3 Sum

## Implementation

### **Running Sum of 1d Array**

***Big O:*** O(n) speed, O(1) space
```
Tips: 

Use the sum of previous item.
```
```c++
class Solution {
public:
    vector<int> runningSum(vector<int>& nums) {
        vector<int> running_sum(nums.size(), 0);
        running_sum[0] = nums[0];
        
        for (int i = 1; i < nums.size(); i++) {
            running_sum[i] = nums[i] + running_sum[i-1];
        }
        
        return running_sum;
    }
};
```

### **Continuous Subarray Sum**

***Big O:*** O(n) speed, O(n) space
```
Tips: 

a%k = x
b%k = x
(a - b) %k = x -x = 0
here a - b = the sum between i and j.
```
```c++
class Solution {
public:
    bool checkSubarraySum(vector<int>& nums, int k) {
        if(nums.size()<2)
            return false;
    
        unordered_map<int, int> mp;
    
        // <0,-1> can allow it to return true when the runningSum%k=0,
        mp[0]=-1;
        
        int runningSum=0;
        for(int i=0;i<nums.size();i++) {
            runningSum+=nums[i];
            
            if(k!=0) 
                runningSum = runningSum%k;
            
            //check if the runningsum already exists in the hashmap
            if(mp.find(runningSum)!=mp.end()) {
                //if it exists, then the current location minus the previous location must be greater than1
                if(i-mp[runningSum]>1)
                    return true;
            }
            else
                mp[runningSum]=i; 
        }
        return false;
    }
};
```

### **Random Pick with Weight**

***Big O:*** O(log(n)) speed, O(n) space
```
Tips: 

Prefix Sums with Binary Search.
```
```c++
class Solution {
    vector<int> sums;
    
public:
    Solution(vector<int>& w) {
        sums = vector<int>(w.size(), 0);
        sums[0] = w[0];
        
        for (int i = 1; i < sums.size(); i++) {
            sums[i] = w[i] + sums[i-1];
        }
    }
    
    int pickIndex() {
        int w_index = rand()%(sums.back());
        
        int st = 0;
        int end = sums.size() - 1;
        int ret;
        
        while (st < end) {
            int mid = st + (end-st)/2;
            
            if (sums[mid] <= w_index) {
                st = mid + 1;
            } else{
                end = mid;
            }
        }
        
        return st;
    }
};

```

### **Friends Of Appropriate Ages**

***Big O:*** O(nlog(n)) speed, O(n) space
```
Tips: 
Approache 1:

Calculate the target value and then do a binary search. For ages we already went through, simply add the number of requests for that age.

Approach 2:

Instead of processing all 20000 people, we can process pairs of (age, count) representing how many people are that age. Since there are only 120 possible ages, this is a much faster loop.
```
```c++
// Approach 2
class Solution {
public:
    int numFriendRequests(vector<int>& ages) {
        int count[121] = {};
        for (int age: ages) count[age]++;

        int ans = 0;
        for (int ageA = 0; ageA <= 120; ageA++) {
            int countA = count[ageA];
            for (int ageB = 0; ageB <= 120; ageB++) {
                int countB = count[ageB];
                if (ageA/2 + 7 >= ageB) continue;
                if (ageA < ageB) continue;
                if (ageA < 100 && 100 < ageB) continue;
                ans += (countA * countB);
                if (ageA == ageB) ans -= countA;
            }
        }

        return ans;
    }
};

// Approach 1
class Solution {
public:
    unordered_map <int,int> map; // key: age, val: FriendRequestCount. 
    
    int findRequests (vector<int> & ages, int index) {
        
        if (map.find(ages[index]) != map.end()) {
            return map[ages[index]];
        }
        
        int left = 0;
        int right = index-1;
        double target = (double) (0.5*ages[index]) + 7; // find ages >= target.
        
        while (left <= right) {
            int mid = left + (right-left)/2;
            if (ages[mid] <= target) {
                left = mid+1;
            } else {
                right = mid-1;
            }
        }
        
        map[ages[index]] = index-left;
        return index-left; // len between index-1 and left. 
    }
    
    int numFriendRequests(vector<int>& ages) {
        
        sort (ages.begin(), ages.end());
        
        int count = 0;
        
        for (int i = ages.size()-1; i >= 0 ; i--) {
            count += findRequests (ages, i);
        }
        return count;
    }
};
```
### **Trap Rain water**

***Big O:*** O(nlog(n)) speed, O(n) space
```
Tips: 
Approache 1:

Use stacks.

Approach 2:

Use two pointers.
```

```c++
// Approach 1: stacks
class Solution {
public:
    int trap(vector<int>& height)
    {
        int ans = 0, current = 0;
        stack<int> st;
        while (current < height.size()) {
            while (!st.empty() && height[current] > height[st.top()]) {
                int top = st.top();
                st.pop();
                if (st.empty())
                    break;
                int distance = current - st.top() - 1;
                int bounded_height = min(height[current], height[st.top()]) - height[top];
                ans += distance * bounded_height;
            }
            st.push(current++);
        }
        return ans;
    }
};

// Approach 2: two pointers 
class Solution {
public:
    int trap(vector<int>& height)
    {
        int ans = 0;
        int left = 0, right = height.size()-1;
        int left_max, right_max;
        left_max = right_max = 0;
        
        while (left < right) {
            right_max = max(right_max, height[right]);
            
            if (height[left] < height[right]) {
                if (left_max > height[left]) {
                    ans += (left_max - height[left]);
                } else {
                    left_max = height[left];
                }
                left ++; 
            } else {
                if (right_max > height[right]) {
                    ans += (right_max - height[right]);
                } else {
                    right_max = height[right];
                }
                right --; 
            }
        }
        
        return ans;
    }
};
```
### **House Robber**

***Big O:*** O(n) speed, O(n) space
```
Tips: 

Dynamic programming with memorization
```
```c++
class Solution {
public:
    int rob(vector<int>& nums) {
        if(nums.size() <= 1) return nums.size() == 0 ? 0 : nums[0];
        
        vector<int> dp(nums.size(), 0);
        
        dp[0] = nums[0];
        dp[1] = max(nums[0], nums[1]);
        
        for (int i = 2; i < nums.size(); i++) {
            dp[i] = max(dp[i-2] + nums[i], dp[i-1]);
        }
        
        return dp[nums.size()-1];
    }
};
```

### **Degree of an array**

***Big O:*** O(n) speed, O(n) space
```
Tips: 

Hash map to record the number frequency and another map to record the postions of each number. when encounter a number with the same degree, calculate the length and take the minium value of them.
```
```c++
class Solution {
public:
    int findShortestSubArray(vector<int>& nums) {
        
    map<int, int> freq;
    map<int, vector<int>> pos;
    int mx = INT_MIN;
    
    /*
    get frequency of each number in array
    get highest degree
    note the positions of frequencies
    */    
    for(int i = 0; i < nums.size(); i++)
    {
        mx = max(mx, ++freq[nums[i]]);        
        pos[nums[i]].push_back(i);
    }
       
    //get shortest distance
    int dist = INT_MAX;
    for(auto num : nums)
    {
        if(freq[num] == mx)            
            dist = min(dist, pos[num].back() - pos[num].front());         
    }
    
    return dist + 1;
        
    }
};
```

### **Decode ways**

***Big O:*** O(n) speed, O(n) space
```
Tips: 

Dynamic programming
```
```c++
class Solution {
public:
    int numDecodings(string s) {
        // edge cases out - leading zero and single character string
        if (s[0] == '0') return 0;
        if (s.size() == 1) return 1;
        // support variables
        int len = s.size(), dp[len];
        // preparing dp
        dp[0] = 1;
        dp[1] = (s[0] == '1' || s[0] == '2' && s[1] < '7' ? 1 : 0) + (s[1] != '0');
        for (int i = 2; i < len; i++) {
            // edge case: we quit for 2 consecutive zeros
            if (s[i] == '0' && (s[i - 1] > '2' || s[i - 1] == '0')) return 0;
            // base case: we always keep the previous set of combinations, unless we met a 0
            dp[i] = s[i] != '0' ? dp[i - 1] : 0;
            // we go and look 2 positions behind if we can make a digit in the 10-26 range
            if (s[i - 1] == '1' || s[i - 1] == '2' && s[i] < '7') dp[i] += dp[i - 2];
        }
        return dp[len - 1];
    }
};
```
### **Best Time to sell stocks II**

***Big O:*** O(n) speed, O(1) space
```
Tips: 

Greedy. Buy low, Sell high.
```
```c++
class Solution {
public:
    int maxProfit(vector<int>& prices) {
        int prev = prices[0], res = 0;
        for (int curr: prices) {
            if (prev < curr) res += curr - prev;
            prev = curr;
        }
        return res;
    }
};
```

### **Monotonic Array**

***Big O:*** O(n) speed, O(1) space
```
Tips: 

Self evident.
```
```c++
class Solution {
public:
    bool isMonotonic(vector<int>& A) {
        bool increase = true;
        bool decrease = true;
        for(int i = 0; i < A.size() - 1; i++) {
            if(A[i] > A[i+1]) increase = false;
            if(A[i] < A[i+1]) decrease = false;
            if(increase == false && decrease == false) return false;
        }
        return true;
    }
};
```

### **Longest Continuous Increasing Subsequence**

***Big O:*** O(n) speed, O(1) space
```
Tips: 

Self evident.
```
```c++
class Solution {
public:
    int findLengthOfLCIS(vector<int>& nums) {
        if(nums.size()<=1)return nums.size();
        int answer=1,count=1;
        for(int i=0;i<nums.size()-1;i++){
            if(nums[i]<nums[i+1]){
                count++;
                answer=max(answer,count);
            }
            else{
                count=1;
            }
        }
        return answer;
    }
};
```
### **Maximum Product Subarray**

***Big O:*** O(n) speed, O(1) space
```
Tips: 

Dynamic Programming. While going through numbers in nums, we will have to keep track of the maximum product up to that number (we will call max_so_far) and minimum product up to that number (we will call min_so_far). The reason behind keeping track of max_so_far is to keep track of the accumulated product of positive numbers. The reason behind keeping track of min_so_far is to properly handle negative numbers.
```
```c++
class Solution {
public:
    
    int maxProduct(vector<int>& nums) {
         if (nums.size() == 0) return 0;

        int max_so_far = nums[0];
        int min_so_far = nums[0];
        int result = max_so_far;

        for (int i = 1; i < nums.size(); i++) {
            int curr = nums[i];
            int temp_max = max(curr, max(max_so_far * curr, min_so_far * curr));
            min_so_far = min(curr, min(max_so_far * curr, min_so_far * curr));

            max_so_far = temp_max;

            result = max(max_so_far, result);
        }

        return result;
    }
};
```

### **First Missing Positive**

***Big O:*** O(n) speed, O(1) space
```
Tips: 

Hashmap. 
```
```c++
class Solution {
public:
    int firstMissingPositive(vector<int>& nums) {
        unordered_map<int, int> umap;
        int max_v = 0;
        
        for (int i = 0; i < nums.size(); i++) {
            if (nums[i] > 0) {
                umap[nums[i]] = 1;
                max_v = max(max_v, nums[i]);
            }
        }
        
        for (int i = 1; i < max_v; i++) {
            if (umap.find(i) == umap.end())
                return i;
        }
        
        return max_v+1;
    }
};
```

### **Maximum Swap**

***Big O:*** O(n) speed, O(1) space
```
Tips: 

Greedy. 
```
```c++
class Solution {
public:
    int maximumSwap(int num) {
        string num_str = to_string(num);
        int n = num_str.size();
        int max_val = -1, max_i = -1;
        int left = -1, right = -1;
        
        for (int i = n - 1; i >= 0; i--) {
            if (num_str[i] > max_val) {
                max_val = num_str[i];
                max_i = i;
            } else if (num_str[i] < max_val) {
                left = i;
                right = max_i;
            }
        }
       
        if (left == -1) return num;
        char c = num_str[left];
        num_str[left] = num_str[right];
        num_str[right] = c;
        return stoi(num_str);
    }
};
```

### **Meeting Rooms II**

***Big O:*** O(nlog(n)) speed, O(n) space
```
Tips: 

Sort + priority queue. Kick people out if their meeting is already finished.
```
```c++
class Solution {
public:
    int minMeetingRooms(vector<vector<int>>& intervals) {
        sort(intervals.begin(), intervals.end());
        
        priority_queue<int, vector<int>, greater<int>> pq;
        
        for (auto v : intervals) {
            // check if the last meetings need to be completed
            // before this meeting starts.
            if (!pq.empty() && pq.top() <= v[0]) {
                pq.pop();
            }
            
            // start this meeting and enter the end time.
            pq.push(v[1]);
        }
        
        return pq.size();
    }
};
```

### **Sort Colors**

***Big O:*** O(n) speed (one pass), O(1) space
```
Tips: 

The idea of solution is to move curr pointer along the array, if nums[curr] = 0 - swap it with nums[p0], if nums[curr] = 2 - swap it with nums[p2].
```
```c++
class Solution {

public: 
    void sortColors(vector<int>& nums) {
        int lo = 0, hi = nums.size() - 1, i = 0;

        while (i <= hi) {
            if      (nums[i] == 0) swap(nums, lo++, i++);
            else if (nums[i] == 2) swap(nums, i, hi--);
            else if (nums[i] == 1) i++;
        }
    }

    void swap(vector<int>& nums, int i, int j) {
        int t = nums[i];
        nums[i] = nums[j];
        nums[j] = t;
    }

};
```

### **3 SUM**

***Big O:*** O(n^2), O(1) space
```
Tips: 

Sort + two pointers.
```
```c++
class Solution {
public:
	vector<vector<int>> threeSum(vector<int>& nums) {
		vector<vector<int>> ans;
		sort(nums.begin(), nums.end());
		int length = nums.size() - 1, left, right;
		for ( int index = 0; index <= length; ++index )
		{
			if ( index > 0 && nums[index - 1] == nums[index] ) continue; 
			left = index + 1;
			right = length;
			while ( left < right )
			{
				if ( nums[index] + nums[left] + nums[right] < 0 ) ++left;
				else if ( nums[index] + nums[left] + nums[right] > 0 ) --right;
				else
				{
					vector<int> anotherAnswer { nums[index], nums[left], nums[right] };
					ans.push_back(anotherAnswer);
					++left;
					while ( left < right && nums[left] == nums[left - 1] ) ++left;
				}   
			}    
		}     
		return ans;
	}
};
```