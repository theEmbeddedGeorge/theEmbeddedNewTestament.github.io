# 2020/2021 Amazon OA questions (Experienced)
## Key algorithm to review before you start

- DFS
- BFS
- Sliding Window
- Tree
- LinkedList.

## Reference

https://www.xiakexing.me/forum.php?mod=viewthread&tid=6336&highlight=Prime%2BAir%2BRoute

https://github.com/neerazz/FAANG/tree/master/Algorithms/Neeraj/topAmazonQuestions

https://aonecode.com/amazon-online-assessment-questions

https://algo.monster/problems/amazon_online_assessment_questions

## Problems List
- [Items in Container](https://aonecode.com/amazon-online-assessment-items-in-containers)
- [Turnstile](https://leetcode.com/discuss/interview-question/699973/Goldman-Sachs-or-OA-or-Turnstile)
- [Friend Circles](https://leetcode.com/problems/number-of-provinces/)
- [Substrings of Size K with K-1 Distinct Chars](https://algo.monster/problems/reorder_data_in_log_files)  
- [Utilization Checks/Auto Scale policy](https://aonecode.com/amazon-online-assessment-utilization-checks)
- [Reorder Data in Log Files](https://leetcode.com/problems/reorder-data-in-log-files/submissions/)
- [Most Common Word  (hashmap)](https://leetcode.com/problems/most-common-word)
- [Robotics Challenge](https://leetcode.com/problems/baseball-game/)
- [Rover Control](https://leetcode.com/discuss/interview-question/985703/Amazon-or-OA-or-Rover-Control)
- [Amazon Go stores/Number of Islands (BFS/DFS)](https://leetcode.com/problems/number-of-islands)
- [Treasure Island](https://leetcode.com/discuss/interview-question/347457/Amazon-or-OA-2019-or-Treasure-Island)
- [Transaction Logs](https://aonecode.com/amazon-online-assessment-transaction-logs)
- [zombie in matrix/Rotting Orange (BFS)](https://www.lintcode.com/problem/zombie-in-matrix/description)
- [Top K Frequently Mentioned Keywords (Priority Queue/Minheap + hash_map)](https://www.lintcode.com/problem/top-k-frequently-mentioned-keywords/)
- [K Closest Points to Origin (Priority Queue/Minheap)](https://leetcode.com/problems/k-closest-points-to-origin)
- [Amazon Music Pairs/Pairs of Songs With Total Durations Divisible by 60](https://leetcode.com/problems/pairs-of-songs-with-total-durations-divisible-by-60)
- [Five Star Sellers](https://algo.monster/problems/five_star_sellers)
- [Optimal Utilization (two pointer)](https://www.lintcode.com/problem/optimalutilization/)
- [Amazon Fresh Promotion](https://leetcode.com/discuss/interview-question/1002811/Amazon-or-OA-2021-or-Fresh-Promotion)
- [LRUcache Count misses](https://jincheng8841.gitbooks.io/leetcode-note/content/lrucache_count_miss.html)


## Implementations

### [K Closest Points to Origin](https://algo.monster/problems/reorder_data_in_log_files)

**Difficulty**: Medium

**Frequency**: High

**Tips**: Priority Queue/Minheap

**Leetcode Link**: https://leetcode.com/problems/k-closest-points-to-origin

### [Items in Container](https://aonecode.com/amazon-online-assessment-items-in-containers)

**Difficulty**: Easy

**Frequency**: High

**Tips**: Two pointer

**Big O**: O(m*n) m: number of indices pair, n: size of string 

```c++
#include <vector>
using namespace std;

// Iterate left to the last '|' character and check how many stars on the way
// If no star or no '|', return 0
int countStar(string &s, int cur, int start) {
    int count{};
    cur--;
    while(cur >= start && s[cur] != '|' ) {
        count = (s[cur] == '*') ? count + 1 : count;
        cur --;
    }
    if (cur < start)
        return 0;
        
    return count;
}

// Iterate through pairs of indices, for each <start, end> pairs, iterate through
// in the right and whenever meets a '|', start "*" check to the left
vector<int> numberOfItems(string s, vector<int>start, vector<int>end) {
    vector<int> output(start.size(), 0);
    
    for (int i = 0; i < start.size(); i++) {
        for (int j = start[i]-1; j <= end[i]-1; j++) {
            if (s[j]=='|') {
                output[i] += countStar(s, j, start[i]-1);
            }
        }
    }

    return output;
}
   
int main() {
    cout << "test1" << endl;
    vector<int> res = numberOfItems("|**|*|*", {1,1}, {5,6});
    for (auto val : res)
        cout << val << endl;
    cout << "test2" << endl;
    res = numberOfItems("*|*|*|", {1}, {6});
    for (auto val : res)
        cout << val << endl;
    cout << "test3" << endl;
    res = numberOfItems("*|*|", {1}, {3});
    for (auto val : res)
        cout << val << endl;
    cout << "test4" << endl;
    res = numberOfItems("*|*|*|", {1}, {6});
    for (auto val : res)
        cout << val << endl;
    cout << "test5" << endl;
    res = numberOfItems("||||*|||*|||*|", {1}, {12});
    for (auto val : res)
        cout << val << endl;
}

```
### [Turnstile](https://leetcode.com/discuss/interview-question/699973/Goldman-Sachs-or-OA-or-Turnstile)

**Difficulty**: Medium

**Frequency**: High

**Tips**: Two Queues

**Big O**: O(n), space: O(n)

**Leetcode Link**: https://leetcode.com/discuss/interview-question/699973/Goldman-Sachs-or-OA-or-Turnstile

```C++
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
  vector<int> solve(vector<int> &t, vector<int> &dir) {
    // Create two queues for entry and exit times
    queue<pair<int, int>> exit, entry; // pair <time[index], index>
    int N = t.size();
    vector<int> res(N);

    for (int i = 0; i < N; i++) {
      if (dir[i] == 1)
        exit.push({t[i], i});
      else
        entry.push({t[i], i});
    }

    int ct = 0, lc = -1; // ct is the current time & lc indicates who used
                         // turnstile in the previous second lc = -1 if none
                         // used the turnstile in the last second

    while (!exit.empty() || !entry.empty()) {
      // checking for exit queue
      if (!exit.empty() && exit.front().first <= ct &&
          (lc == 1 || lc == -1 || entry.empty() ||
           (entry.front().first > ct))) {
        res[exit.front().second] = ct;
        lc = 1;
        exit.pop();
      }
      // checking for entry queue
      else if (!entry.empty() && entry.front().first <= ct) {
        res[entry.front().second] = ct;
        lc = 0;
        entry.pop();
      } else {
        lc = -1;
      }

      ct++;
    }

    return res;
  }
};

static int i = 1;
void test(Solution &sol, vector<int> &t, vector<int> &dir,
          vector<int> &expected) {
  cout << "Run test case " << i++ << endl;
  auto res = sol.solve(t, dir);
  for (auto &item : res)
    cout << item << " ";
  cout << endl;
  for (auto &item : expected)
    cout << item << " ";
  cout << endl;

  assert(res == expected);
  cout << "Success!" << endl;
}

int main() {
  Solution sol;
  vector<int> t = {0, 0, 1, 5};
  vector<int> dir = {0, 1, 1, 0};
  vector<int> expected = {2, 0, 1, 5};
  test(sol, t, dir, expected);

  vector<int> t2 = {1, 2, 4};
  vector<int> dir2 = {0, 1, 1};
  vector<int> expected2 = {1, 2, 4};
  test(sol, t2, dir2, expected2);

  vector<int> t3 = {1, 1};
  vector<int> dir3 = {1, 1};
  vector<int> expected3 = {1, 2};
  test(sol, t3, dir3, expected3);

  vector<int> t4 = {1, 1, 3, 3, 4, 5, 6, 7, 7};
  vector<int> dir4 = {1, 1, 0, 0, 0, 1, 1, 1, 1};
  vector<int> expected4 = {1, 2, 3, 4, 5, 6, 7, 8, 9};
  test(sol, t4, dir4, expected4);

  return 0;
}
```
### [Friend Circles](https://algo.monster/problems/friend_circles)

**Difficulty**: Medium

**Frequency**: High

**Tips**: dfs

**Big O**: O(n^2) space: O(n)

**Leetcode Link**: https://leetcode.com/problems/number-of-provinces/

```c++
class Solution {
public:
    int findCircleNum(vector<vector<int>>& M) {
        int n=M.size(),ans=0;
        if(n==0) return 0;

        vector<bool>vis(n,false);

        for(int i=0;i<n;i++)
        {
            if(!vis[i])
            {
                ans++;
                dfs(M,vis,i);
            }
        }
        return ans;
    }

    void dfs(vector<vector<int>>& M, vector<bool>& vis, int i)
    {
        vis[i]=true;
        for(int j=0;j<M.size();j++)
            if(M[i][j]==1 && !vis[j])
                dfs(M,vis,j);
    }
};
```

### [Substrings of Size K with K-1 Distinct Chars](https://algo.monster/problems/reorder_data_in_log_files)

**Difficulty**: Medium

**Frequency**: High

**Tips**: Sliding window + Hashset

**Big O**: O(n), space: O(n)

**Leetcode Link**: https://www.lintcode.com/problem/k-substring-with-k-different-characters/solution
```c++
class Solution {
public: 
    int KSubstring(string &stringIn, int K) {
        // Write your code here
        int Map[5000]; // Implement a hash to record how character appearance counts
        memset( Map , 0 , sizeof(Map));
        set<string> s;
        int cnt = 0,res = 0;
        // Count character counts for the first K
        for(int i = 0 ; i < K && i < stringIn.length() ; i++) {
            if(!Map[int(stringIn[i])]) {
                cnt ++ ;
            }
            Map[int(stringIn[i])] ++ ;
        }
        // ans ++ if the first K has distinctive characters
        if(cnt == K) {
            s.insert(stringIn.substr(0 , K));
        } 
        // Do sliding window
        for(int i = K; i < stringIn.length() ; i++) {
            Map[int(stringIn[i - K])] -- ;
            if(! Map[int(stringIn[i - K])])
                cnt -- ;
            if(! Map[int(stringIn[i])]) {
                cnt ++ ;
            }
            Map[int(stringIn[i])] ++ ;
            if(cnt == K) {
                s.insert(stringIn.substr(i - K + 1 , K));
            }
        }
        return s.size();
    }
};
```	

### [Utilization Checks](https://aonecode.com/amazon-online-assessment-utilization-checks)

**Difficulty**: Easy

**Frequency**: High

**Tips**:

**Big O**: O(n)

```c++
#include <math.h> 
using namespace std;

int utilizationChecks (int instances, vector<int> averageUtil) {
    for (int i = 0; i <= averageUtil.size(); i++) {
        int item = averageUtil[i];
        if (item < 25 && instances > 1) {
            instances = ceil(instances/2.0);
            i += 10;
        } else if (item > 60 && instances < 2*pow(10,8)) {
            instances *= 2;
            i += 10;
        }
    }
    
    return instances;
}

int main() {
    cout << utilizationChecks(2, {25, 23, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 76, 80}) << endl;
    cout << utilizationChecks(1, {5, 10, 80}) << endl;
    cout << utilizationChecks(5, {30, 5, 4, 8, 19, 89}) << endl;
}
```

### [Reorder Data in Log Files](https://algo.monster/problems/reorder_data_in_log_files)

**Difficulty**: Easy

**Frequency**: High

**Tips**: two queues + sort

**Big O**: O(nlog(n)) space: O(n)

**Leetcode Link**: https://leetcode.com/problems/reorder-data-in-log-files

```c++
bool compare(const string &A, const string &B) {
    // strip the identifier
    string cont1 = A.substr(A.find(' ')+1);
    string cont2 = B.substr(B.find(' ')+1);
    
    // if content is equal, compare the identifier
    return cont1 == cont2 ? A < B : cont1 < cont2;
}

class Solution {
public:
    vector<string> reorderLogFiles(vector<string>& logs) {
        vector<string> ret;
        vector<string> let_log, dig_log;
        
        // divide all logs into letter logs and digit logs
        for (auto str : logs) {
            if (isalpha(str[str.size()-1]))
                let_log.push_back(str);
            else
                dig_log.push_back(str);
        }
        
        // sort letter logs according to the compare rules specified
        sort(let_log.begin(), let_log.end(), compare);
        ret = vector<string>(let_log.begin(), let_log.end());
        
        /*for (auto str : dig_log) {
            ret.push_back(str);
        }*/
        ret.insert(ret.end(), dig_log.begin(), dig_log.end()); // insert digit logs after letter logs
        
        return ret;
    }
};
```

### [Most Common Word](https://algo.monster/problems/most_common_word)

**Difficulty**: Easy

**Frequency**: High

**Tips**: Hashmap + hashset

**Big O**: O(n) space: O(n)

**Leetcode Link**: https://leetcode.com/problems/most-common-word

```c++
string mostCommonWord(string paragraph, vector<string>& banned) {

    // string preprocessing, turn all punctuation into space, and all upper letter to lower case
	for(auto&c : paragraph)
		c = ispunct(c) ? ' ' : tolower(c);

    // create hash map to record string appearance frequency
    // and hash set to store ban word lists 
	unordered_map<string, int> freq;
	unordered_set<string> b(banned.begin(), banned.end());
	stringstream ss(paragraph);
    string str, res;

    // for words in paragraph, if not present in ban list and most frequent words, record it
	while(ss>>str)
		if(b.count(str)==0 && freq[res]<++freq[str]) 
			res = str;

	return res;
}
```

### [Robotics Challenge](https://leetcode.com/problems/baseball-game/submissions/)

**Difficulty**: Easy

**Frequency**: High

**Tips**: Stack

**Big O**: O(n) space: O(n)

**Leetcode Link**: https://leetcode.com/problems/baseball-game/submissions/
```c++
class Solution {
public:
    int calPoints(vector<string>& ops) {
        stack<int> scores;
        int ret{};
        
        for (auto str : ops) {
           if (isdigit(str[str.size()-1])) {
               scores.push(stoi(str));
           } else if (str[0] == 'C') {
               scores.pop();
           } else if (str[0] == 'D') {
               scores.push(scores.top()*2);
           } else if (str[0] == '+') {
               int top = scores.top();
               scores.pop();
               int val = scores.top() + top;
               scores.push(top);
               scores.push(val);
           }
        }
        
        while(!scores.empty()) {
            ret += scores.top();
            scores.pop();
        }
        
        return ret;
    }
};
```
### [Rover Control](https://aonecode.com/amazon-online-assessment-rover-control)

**Difficulty**: Easy

**Frequency**: High

**Tips**: hashmap

**Big O**: O(n) space: O(n)

**Leetcode link:** https://github.com/LacticAcidCYC/CS_Interview_Notes/blob/master/OA/Twitter_OA.md

```c++
// Rover Control
int roverMove(int size, vector<string> cmds);

int roverMove(int size, vector<string> cmds) {
    int pi = 0, pj = 0;
    unordered_map<string, vector<int>> moves{
        {"UP", {-1, 0}},
        {"RIGHT", {0, 1}},
        {"DOWN", {1, 0}},
        {"LEFT", {0, -1}}
    };
    
    for (auto cmd : cmds) {
        int ni = pi + moves[cmd][0];
        int nj = pj + moves[cmd][1];
        if (ni >= 0 && ni < size && nj >= 0 && nj < size) {
            pi = ni;
            pj = nj;
        }
    }
    // calculate how many steps has been moved
    return pi*size+pj;
}
```

### [Number of Islands](https://leetcode.com/problems/number-of-islands/)

**Difficulty**: Medium

**Frequency**: Very High

**Tips**: BFS/DFS

**Big O**: O(n+m)

```c++
// DFS
class Solution {
public:
    
    void dfs(vector<vector<char>> &grid, int i, int j, int m, int n) {
        if(i>=0 && i<m && j>=0 && j<n && grid[i][j] == '1') {
            grid[i][j] = 0;
            dfs(grid, i-1, j, m, n);
            dfs(grid, i+1, j, m, n);
            dfs(grid, i, j-1, m, n);
            dfs(grid, i, j+1, m, n);
        }
        else
            return;
            
    }
    
    int numIslands(vector<vector<char>>& grid) {
        
        if(grid.size() == 0)
            return 0;
        
        int m = grid.size();
        int n = grid[0].size();
        
        int ans = 0;
        
        for(int i=0;i<m;i++) {
            for(int j=0;j<n;j++) {
                if(grid[i][j] == '1') {
                    ans++;
                    dfs(grid, i, j, m, n);
                }
            }
        }
        
        return ans;
    }
};

// BFS
class Solution {
public:
    int numIslands(vector<vector<char>>& grid) {
        int count{};
        
        int m = grid.size();
        int n = grid[0].size();
        vector<vector<int>> visited(m, vector<int>(n, 0));
        queue<pair<int, int>> islands;

        for (int y = 0; y < m; y++) {
            for (int x = 0; x < n; x ++) {
                if (grid[y][x] == '0') continue;
                else if (grid[y][x] == '1' && visited[y][x] == 1) continue;
                else if (grid[y][x] == '1' && visited[y][x] == 0) {
                    count ++;
                }
                
                //cout << y << x << endl;
                islands.push(make_pair(y, x));
                visited[y][x] = 1;
                while(!islands.empty()) {
                    int size = islands.size();
                    while(size--) {
                        pair<int, int> land = islands.front();
                        islands.pop();
                        
                        int yy = land.first;
                        int xx = land.second;
                        //cout << yy << xx << endl;
                        
                        if (yy-1 >= 0 && !visited[yy-1][xx] && grid[yy-1][xx] == '1') {
                            islands.push(make_pair(yy-1, xx));
                            visited[yy-1][xx] = 1;
                        }
                        if (yy+1 < m && !visited[yy+1][xx] && grid[yy+1][xx] == '1') {
                            islands.push(make_pair(yy+1, xx));
                            visited[yy+1][xx] = 1;
                        }
                        if (xx-1 >= 0 && !visited[yy][xx-1] && grid[yy][xx-1] == '1') {
                            islands.push(make_pair(yy, xx-1));
                            visited[yy][xx-1] = 1;
                        }
                        if (xx+1 < n && !visited[yy][xx+1] && grid[yy][xx+1] == '1') {
                            islands.push(make_pair(yy, xx+1));
                            visited[yy][xx+1] = 1;
                        }       
                    }
                }
            }
        }
        
        return count;
    }
};
```

### [Treasure Islands](https://algo.monster/problems/treasure_island_i)

**Difficulty**: Medium

**Frequency**: High

**Tips**: BFS/DFS

**Big O**: O(n+m)

```C++
#include <iostream>
#include <vector>
#include <queue>
#include <utility>

using namespace std;

// BFS
int minStepsBFS(vector<vector<char>>& island) {
    vector<vector<int>> dirs = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    int m = island.size(), n = island[0].size();

    queue<pair<int, int>> q;
    q.push(std::pair<int, int>(0, 0)); 
    island[0][0] = 'D';

    int steps = 1;
    while (!q.empty()) {
        int levelSize = q.size();
        for (int i = 0; i < levelSize; ++i) {
            auto pos = q.front();
            q.pop();
            for (auto dir: dirs) {
                int nx = pos.first + dir[0];
                int ny = pos.second + dir[1];
                if (nx < 0 || nx >= m || ny < 0 || ny >= n || island[nx][ny] == 'D') continue;
                if (island[nx][ny] == 'X') return steps;
                q.push(pair<int,int>(nx, ny));
                island[nx][ny] = 'D';
            } 
        }
        
        ++steps;
    }

    return -1;
}

// DFS
int res = INT_MAX;

void dfs(vector<vector<char>>& island, const vector<vector<int>>& dirs, int x, int y, int steps) { 
    if (x < 0 || x >= island.size() || y < 0 || y >= island[0].size() || island[x][y] == 'D' || steps > res) 
        return;
    if (island[x][y] == 'X') {
        res = std::min(res, steps);
        return;
    }

    island[x][y] = 'D';
    for (auto dir: dirs) {
        int nx = x + dir[0];
        int ny = y + dir[1];
        dfs(island, dirs, nx, ny, steps+1);
    }
}

int minStepsDFS(vector<vector<char>>& island) {
    vector<vector<int>> dirs = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    dfs(island, dirs, 0, 0, 0);
    return res == INT_MAX ? -1 : res;
}

int main() {
    vector<vector<char>> island = {{'O', 'O', 'O', 'O', 'O'},
                                   {'D', 'D', 'D', 'O', 'O'},
                                   {'O', 'O', 'O', 'O', 'D'}, 
                                   {'O', 'D', 'X', 'D', 'O'}};

    vector<vector<char>> islandcopy(island);
    cout << minStepsBFS(island) << endl;
    cout << minStepsDFS(islandcopy) << endl;
}
```

### [Transaction Logs](https://aonecode.com/amazon-online-assessment-transaction-logs)

**Difficulty**: Easy

**Frequency**: Very High

**Tips**: hashmap + sort

**Big O**: O(nlog(n)) space: O(n)

**Leetcode links:** https://leetcode.com/discuss/interview-question/989768/Amazon-or-OA-2020-or-Transaction-logs
```c++
vector<string> get_fraud_id(vector<string>& uid, int threshold) {
    vector<string> result;
    unordered_map <string, int> ump;
    
    // iterate through logs, if id1 != id2, transactions for both id increment
    // otherwise only increment one
    for (auto str : uid) {
        vector<string> temp;
        stringstream ss(str);
        string word;
        while (ss >> word) {
            temp.push_back(word);
        }
        if (temp[0] != temp[1]) {
            ump[temp[0]]++;
            ump[temp[1]]++;
        }
        else
            ump[temp[0]]++;
    }
    
    for (auto i1 : ump) {
        if (i1.second >= threshold)
            result.push_back(i1.first);
    }

    // display restult based on id in ascending order
    sort(result.begin(), result.end(), [](const string& a, const string& b)
                                            {return stoi(a) > stoi(b); });

    return result;
}

int main() {

    vector<string> id = {"345366 89921 45", "029323 38239 23",
                         "38239 345366 15", "029323 38239 77",
                         "345366 38239 23", "029323 345366 13",
                         "38239 38239 23"};
    vector<string> res(get_fraud_id(id, 3));
    for (auto &s1: res)
        cout<<s1<<" ";

    return 0;
}
```
### [Zombie in Matrix/Rotting Oranges](https://algo.monster/problems/zombie_matrix)

**Difficulty**: Medium

**Frequency**: Medium

**Tips**: BFS

**Big O**: O(n+m)

**leetcode link:** https://www.lintcode.com/problem/zombie-in-matrix/

```c++
class Position {
public:
    int x, y;
    Position(int xx, int yy):x(xx), y(yy) {}
};

class Solution {
public:
    /**
     * @param grid  a 2D integer grid
     * @return an integer
     */
    int zombie(vector<vector<int>>& grid) {
        // Write your code here
        queue<Position> q;
        int n = grid.size();
        if (n == 0)
            return 0;
        int m = grid[0].size();
        if (m == 0)
            return 0;

        for (int i = 0; i < n; ++i)
            for (int j = 0; j < m; ++j)
                if (grid[i][j] == 1)
                    q.push(Position(i, j));

        int d[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
        int days = 0;
        while (!q.empty()) {
            days ++;
            int size = q.size();
            for (int i = 0; i < size; i++) {
                Position head = q.front();
                q.pop();
                for (int k = 0; k < 4; ++k) {
                    int x = head.x + d[k][0];
                    int y = head.y + d[k][1];
                    if (x >= 0 && x < n && y >=0 && y < m && grid[x][y] == 0) {
                        grid[x][y] = 1;
                        q.push(Position(x, y));
                    }
                }
            }
        }

        for (int i = 0; i < n; ++i)
            for (int j = 0; j < m; ++j)
                if (grid[i][j] == 0)
                    return -1;

        return days - 1;
    }
};
```

### [Music Pairs](https://algo.monster/problems/pairs_of_songs)

**Difficulty**: Medium

**Frequency**: Very High

**Tips**: Modulous

**Big O**: O(n) space: O(n)

**leetcode link:** https://leetcode.com/problems/pairs-of-songs-with-total-durations-divisible-by-60/submissions/

```c++
class Solution {
public:
    /**
     * @param musics: the musics
     * @return: calc the number of pair of music
     */
    long long musicPairs(vector<int> &musics) {
        vector<int> ms(60, 0);
        long long ans{};
        
        // For each number, we simply need to count ms[60-cur] to know how many
        // pairs are divisible by 60
        for (int i = 0; i < musics.size(); i++) {
            int curr = musics[i] % 60;
            ans += (curr == 0) ? ms[0] : ms[60 - curr];
            ms[curr] ++;
        }
        
        return ans;
    }
};
```

### [Top K Frequently Mentioned Keywords](https://algo.monster/problems/top_k_frequently_mentioned_keywords)

**Difficulty**: Medium

**Frequency**: High

**Tips**: Hashmap + PriorityQueue

**Big O**: O(n) space: O(n)

**leetcode link:** https://leetcode.com/problems/top-k-frequent-words/submissions/

```c++
class compare {
    public:
        bool operator()(pair<int, string> A, pair<int, string> B) {
            return (A.first == B.first) ? A.second > B.second : A.first < B.first;
        }  
};

class Solution {
public:
    vector<string> topKFrequent(vector<string>& words, int k) {
        unordered_map<string, int> umap;
        priority_queue<pair<int, string>, vector<pair<int, string>>, compare> pq;
        vector<string> ret;
        
        // Record word appearance frequency
        for (auto str : words)
            umap[str] ++;
        
        // Push them into priority queue (Maxheap)
        for (auto it : umap)
            pq.push(make_pair(it.second, it.first));
        
        // push the first k items into the return vector
        while(k-- && !pq.empty()) {
            ret.push_back(pq.top().second);
            pq.pop();
        }
        
        return ret;
    }
};
```

### [Five Star Sellers](https://aonecode.com/amazon-online-assessment-five-star-sellers)

**Difficulty**: Medium

**Frequency**: High

**Tips**: 

**Big O**: O(n^2)

```C++
int fiveStarReviews(vector<vector<int>> data, int ratingsThreshold) {
    int result = 0;
    int totalProducts = data.size();
    // calculate the threashold value Sum
    double reqSum = ratingsThreshold * totalProducts * 1.0 / 100;
    double cSum = 0;
    
    // while current score sum is smaller than threshold sum
    while(cSum < reqSum){
        cSum = 0;
        double maxCon = 0;
        int productItem = -1;
        for (int i = 0; i < data.size(); i++)
        {
            // add a five star to ith product
            // test which product we should add a five star to so that
            // can maximize the score
            double c = (data[i][0] + 1)* 1.0/ (data[i][1] + 1) - data[i][0] * 1.0/ data[i][1];
            if(maxCon < c){
                maxCon = c;
                productItem = i;
            }
            // calculate current scores
            cSum += data[i][0] * 1.0 / data[i][1];
        }
        
        // subtract the old score for the product item and add the score after adding a five-star score
        cSum = cSum - data[productItem][0] * 1.0 / data[productItem][1];
        data[productItem][0] = data[productItem][0] + 1;
        data[productItem][1] = data[productItem][1] + 1;
        cSum = cSum + data[productItem][0] * 1.0 / data[productItem][1];
        result++;
    }
    
    return result;
}

int main() {

    vector<vector<int>> data = {{4,4},{1,2},{3,6}};
    int result = fiveStarReviews(data, 77);
    cout << result << endl;
}
```
### [Optimal Utilization](https://www.lintcode.com/problem/1797/)

**Difficulty**: Easy

**Frequency**: High

**Tips**: two pointer

**Big O:** O(n^2)

```C++
#include <vector>
#include <algorithm>
#include <iterator>
#include <iostream>

using namespace std;

vector<vector<int>> optimal(vector<vector<int>>& a, vector<vector<int>>& b, int target)
{
    // sort two arrays base on their item values
    std::sort(a.begin(), a.end(), [](vector<int>& a, vector<int>& b) { return a[1] < b[1];});
    std::sort(b.begin(), b.end(), [](vector<int>& a, vector<int>& b) { return a[1] < b[1];});

    auto aptr = a.begin();
    auto bptr = b.rbegin();

    int minSum = INT_MIN;
    vector<vector<int>> res;

    while(aptr != a.end() && bptr != b.rend())
    {
        // calculate the sum from the smallest
        int sum = (*aptr)[1] + (*bptr)[1];
        if(sum > target)
            //increment bptr
            ++bptr;
        else
        {
            //sum <= to target which is what we want
            if(sum > minSum)
            {
                res.clear();
            }
            res.push_back({(*aptr)[0], (*bptr)[0]});
            minSum = sum;
            ++aptr;
        }
    }
    return res;
}

void test(vector<vector<int>> a, vector<vector<int>> b, int target)
{
    vector<vector<int>> res = optimal(a, b, target);
    for(auto i = res.begin(); i != res.end(); ++i)
    {
        for(auto j = (*i).begin(); j != (*i).end(); ++j)
            cout << ' ' << *j;
        cout << '\n';
    }

    cout << endl;
}

int main()
{
    test({{1, 2}, {2, 4}, {3, 6}}, {{1, 2}}, 7);
    test({{1, 3}, {2, 5}, {3, 7}, {4, 10}}, {{1, 2}, {2, 3}, {3, 4}, {4, 5}}, 10);
    test({{1, 8}, {2, 7}, {3, 14}}, {{1, 5}, {2, 7}, {3, 14}}, 20);
    return 0;
}
```

### [Amazon Fresh Promotion](https://leetcode.com/discuss/interview-question/1002811/Amazon-or-OA-2021-or-Fresh-Promotion)

**Difficulty**: Medium

**Frequency**: High

**Tips**:

**Big O**: O(n^2)

```c++
bool hasOrder(vector<string> shoppingCart, int idx, vector<string> order) {
    // Loop through the codeList to check if the fruits are in order.
    for (auto s : order) {
        if (idx < shoppingCart.size() && (s == "anything" || shoppingCart[idx] == s)){
            idx ++;
        } else
            return false;
    }
    return true;
}

int freshPromotion(vector<vector<string>> codeList, vector<string> shoppingCart) {
    // Start at 0 index for both the code list and shopping cart.
    int cartIdx = 0, codeIdx = 0;
    string cur;
    while (cartIdx < shoppingCart.size() && codeIdx < codeList.size()) {
        cur = shoppingCart[cartIdx];

        // If the first fruit of the codeList is anything or if it matches the current fruit at the cart idx.
        if ((codeList[codeIdx][0] == "anything" || codeList[codeIdx][0] == cur) && hasOrder(shoppingCart, cartIdx, codeList[codeIdx])){
            cartIdx += codeList[codeIdx++].size();
        } else
            cartIdx ++;
    }
    // If the all the codeList is present then return 1, else 0.
    return codeIdx == codeList.size() ? 1 : 0;
}

int main() {
    // example 1:
    vector<vector<string>> codeList = {{"apple", "apple"}, {"banana", "anything", "banana"}};
    vector<string> shoppingCart = {"orange", "apple", "apple", "banana", "orange", "banana"};
    cout << freshPromotion(codeList, shoppingCart) << endl;
    
    // example 2:
    codeList = {{"apple", "apple"}, {"banana", "anything", "banana"}};
    shoppingCart = {"banana", "orange", "banana", "apple", "apple"};
    cout << freshPromotion(codeList, shoppingCart) << endl;
    
    // example 3:
    codeList = {{"apple", "apple"}, {"banana", "anything", "banana"}};
    shoppingCart = {"apple", "banana", "apple", "banana", "orange", "banana"};
    cout << freshPromotion(codeList, shoppingCart) << endl;
    
    // example 4:
    codeList = {{"apple", "apple"}, {"apple", "apple", "banana"}};
    shoppingCart = {"apple", "apple", "apple", "banana"};
    cout << freshPromotion(codeList, shoppingCart) << endl;
}
```
### [LRUcache count misses](https://jincheng8841.gitbooks.io/leetcode-note/content/lrucache_count_miss.html)

**Difficulty**: Medium

**Frequency**: High

**Tips**: 

**Big O:**

```C++
class Solution {
  public:
    int countMiss(vector<int> input, int size) {
      unordered_map<int, int> map;
      priority_queue<pair<int, int> > minHeap;
      int n = input.size();
      int count = 0;
      for ( int i = 0; i < n; i++ ) {
        if ( map.find(input[i]) == map.end() and map.size() < size ) {
          // case of count
          map[input[i]] = i;
          minHeap.push(pair<int, int>(-i, input[i]));
          count += 1;
          continue;
        }
        if ( map.find(input[i]) != map.end() ) {
          // case of hit
          map[input[i]] = i; // will update in heap in later manipulation;
          continue;
        }
        while ( true ) {
          int j = -minHeap.top().first, val = minHeap.top().second;
          minHeap.pop();
          if ( map[val] != j ) minHeap.push(pair<int, int>(-map[val], val));
          else {
            map.erase(map.find(val));
            break;
          }
        }
        map[input[i]] = i;
        minHeap.push(pair<int, int>(-i, input[i]));
        count += 1;
      }
      return count;
    }
};

int main() {
  Solution sol;
  int arr[7] = {1,2,3,4,5,4,1};
  vector<int> input(arr, arr + sizeof(arr)/sizeof(arr[0]));
  int size = 5;
  int res = sol.countMiss(input, size);
  cout << res << endl;
  return 0;
}
```

