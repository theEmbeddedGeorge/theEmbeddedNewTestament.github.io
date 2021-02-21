## Problems

1. Running Sum of 1d Array
2. Continuous Subarray Sum
3. Random Pick with Weight
4. Friends Of Appropriate Ages
5. Trap Rain Water
6. House Robber


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