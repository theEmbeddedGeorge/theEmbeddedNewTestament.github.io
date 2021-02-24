## Problems

1. Valid Palindrome II                         
2. Valid Palindrome
3. Decode String
4. Find All Anagrams in a String                     

## Implementation

### **Valid Palindrome**

***Big O:*** O(n), O(1) space
```
Tips: 

Two pointers.
```
```c++
class Solution {
public:
    bool isPalindrome(string s) {
        int st = 0;
        int end = s.size()-1;
        
        while (st < end) {
            if (!isalnum(s[st]) || !isalnum(s[end])) {
                while(st < end && !isalnum(s[st])) {
                    st ++;
                }
                while(st < end && !isalnum(s[end])) {
                    end --;
                }
            } else {
                if ((isalpha(s[st]) && isalpha(s[end]) && tolower(s[st]) == tolower(s[end])) || (s[st] - '0' == s[end] - '0')) {
                    st ++;
                    end --;
                }  else 
                    return false;
            } 
        }
        
        return true;
    }
};
```
### **Valid Palindrome II  **

***Big O:*** O(n), O(1) space
```
Tips: 

Implement is_palindrome first. return the answer of two is_palin or together.
```
```c++
class Solution {

public:
    // Checks if string is a palindrome
    bool isPalin(string &s, int start, int end) {
        while(start < end) {
            if(s[start] != s[end])
                return false;
            ++start, --end;
        }
        return true;
    }
    
    // TC: O(N)
    // SC: O(1)
    bool validPalindrome(string s) {
        for(int i = 0, j = s.size()-1; i < j;  ++i, --j) {
            // mismatch found, only if it is the first time delete
            // a char and move on, else not possible
            if(s[i] != s[j]) {
                // s[0:i-1] and s[j+1, n-1] matched,
                // now we check if atleast s[i:j-1] or s[i+1:j] is a palindrome
                return (isPalin(s, i, j-1) || isPalin(s, i+1, j));
            }
        }
        return true;
    }
};
```

### **Decode String**

***Big O:*** 

Time Complexity: O(maxK⋅n), where maxK is the maximum value of kk and nn is the length of a given string ss. We traverse a string of size nn and iterate kk times to decode each pattern of form k[string]. This gives us worst case time complexity as O(maxK⋅n).

Space Complexity: O(m+n), where mm is the number of letters(a-z) and nn is the number of digits(0-9) in string ss. In worst case, the maximum size of stringStack and countStack could be mm and nn respectively.
```
Tips: 

Use stack to store pair<int, int>(frequency, string). (Equivalent of using two stacks).
```
```c++
class Solution {
public:
    string decodeString(string s) {
        stack<pair<int, string>> decode_stk;
        string ret = "";
        
        int number = 0;
        for (int i = 0; i < s.size(); i++) {
            if (s[i] == '[') {
                string encode_str = "";
                decode_stk.push(make_pair(number, encode_str));
                number = 0;
            } else if (s[i] == ']') {
                pair<int, string> tmp = decode_stk.top(); 
                decode_stk.pop();
                
                string copy = tmp.second;
                while(--tmp.first) {
                    copy += tmp.second;
                }
                
                if (decode_stk.empty()) {
                    ret += copy;
                } else {
                    tmp = decode_stk.top();
                    decode_stk.pop();
                    tmp.second += copy;
                    decode_stk.push(tmp);
                }
            } else if (isalpha(s[i])) {
                if (decode_stk.empty()) {
                    ret += s[i];
                } else {
                    pair<int, string> tmp = decode_stk.top(); 
                    decode_stk.pop();
                    tmp.second += s[i];
                    decode_stk.push(tmp);
                }
            } else if (isdigit(s[i])) {
                number = number*10 + s[i] - '0';
            }
        }
        
        return ret;
    }
};
```
### **Find All Anagrams in a String**

***Big O:*** O(Ns + Np), O(1) space (no more than 26 characters)
```
Tips: 

Sliding Window with HashMap or Sliding Window with Array with 26 entries
```
```c++
class Solution {
public:
    vector<int> findAnagrams(string s, string p) {
        unordered_map<char, int> p_umap, s_umap;
        vector<int> ret;
        
        if (s.size() < p.size())
            return ret;
        
        for (auto c : p)
            p_umap[c] ++;
        
        int st = 0, end = 0;
        
        while (end < s.size()) {
            //cout << st << end << endl;
            if (end-st+1 < p.size()) {
                if (p_umap.find(s[end]) != p_umap.end()) {
                    s_umap[s[end]] ++;
                    end ++;
                } else {
                    s_umap.clear();
                    end ++;
                    st = end;
                }
            } else if ((end-st+1 > p.size())) {
                if (s_umap[s[st]] == 1)
                    s_umap.erase(s[st]);
                else
                    s_umap[s[st]] --;
                st ++;
            } else {
                s_umap[s[end]] ++;
                if (s_umap == p_umap)
                    ret.push_back(st);
                end ++;
            }
        }
        
        return ret;
    }
};
```