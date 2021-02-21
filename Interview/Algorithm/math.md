## Problems

1. Reverse Integer without using long long
2. Palindrome Number
3. Dot Product of Two Sparse Vectors


## Implementation
### **Dot Product of Two Sparse Vectors**

***Big O:*** O(k) speed - k is the size of the array with smaller length, O(n) space
```
Tips: 

Use hashmap to store <index, value>. While performing the dot product, iterate through the hashmap of the more sparse array and do the sum.
```
```c++
class SparseVector {
public:
    unordered_map<int, int> umap;
    
    SparseVector(vector<int> &nums) {
        for (int i = 0; i < nums.size(); i++) {
            if (nums[i] != 0)
                umap[i] = nums[i];
        }
    }
    
    // Return the dotProduct of two sparse vectors
    int dotProduct(SparseVector& vec) {
        int sum = 0;
        
        if (vec.umap.size() < this->umap.size()) {
            for (auto it : vec.umap) {
                if (umap.find(it.first) != umap.end())
                    sum += umap[it.first]*it.second;
            }
        } else {
             for (auto it : umap) {
                if (vec.umap.find(it.first) != vec.umap.end())
                    sum += vec.umap[it.first]*it.second;
            }
        }
        
        return sum;
    }
};

```

### **Reverse Integer without using long long**

***Big O:*** O(n) speed, O(1) space
```
Tips: 

Check overflow by comparing with boundary value divided by 10. 

Another approach could be first casting the integer to string and then do the comparison on strings.
```
```c++
class Solution {
public:
    int reverse(int x) {
        int y=0;
        while(x){
            if(y>INT_MAX/10 || y<INT_MIN/10){
                return 0;
            }else{
                y=y*10 +x%10;
                x=x/10;
            }
        }
        return y;
    }
};
```

## **Palindrome Number**

***Big O:*** O(n) speed, O(1) space
```
Tips: 

Reverse integer and compare it with the original one. Return true if equal. Be careful with overflow. Use long long for the reversed number variable.
```
```c++
class Solution {
public:
    bool isPalindrome(int x) {
        if (x < 0)
            return false;
        
        long long temp = x, dummy = 0;
        
        while (temp) {
            dummy *= 10;
            dummy += temp%10;
            temp /= 10;
        }
        
        return (long long) x == dummy;
    }
};

```