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
- [Five Star Sellers](https://algo.monster/problems/five_star_sellers)
- [Robotics Challenge](https://leetcode.com/problems/baseball-game/)
- [Transaction Logs](https://aonecode.com/amazon-online-assessment-transaction-logs)
- [Most Common Word  (hashmap)](https://leetcode.com/problems/most-common-word)
- [Rotting Orange (BFS)](https://leetcode.com/problems/rotting-oranges)
- [zombie in matrix (BFS)](https://www.lintcode.com/problem/zombie-in-matrix/description)
- [Amazon Music Pairs/Pairs of Songs With Total Durations Divisible by 60](https://leetcode.com/problems/pairs-of-songs-with-total-durations-divisible-by-60)
- [Rover Control](https://leetcode.com/discuss/interview-question/985703/Amazon-or-OA-or-Rover-Control)
- [Amazon Go stores/Number of Islands (BFS/DFS)](https://leetcode.com/problems/number-of-islands)
- [Top K Frequently Mentioned Keywords (Priority Queue/Minheap + hash_map)](https://www.lintcode.com/problem/top-k-frequently-mentioned-keywords/)
- [Amazon Fresh Promotion](https://leetcode.com/discuss/interview-question/1002811/Amazon-or-OA-2021-or-Fresh-Promotion)
- [Reorder Data in Log Files](https://leetcode.com/problems/reorder-data-in-log-files/submissions/)
- [K Closest Points to Origin (Priority Queue/Minheap)](https://leetcode.com/problems/k-closest-points-to-origin)
- [Utilization Checks](https://aonecode.com/amazon-online-assessment-utilization-checks)
- [Substrings of Size K with K-1 Distinct Chars](https://algo.monster/problems/reorder_data_in_log_files)
- [Friend Circles](https://leetcode.com/problems/number-of-provinces/)
- [Turnstile](https://leetcode.com/discuss/interview-question/699973/Goldman-Sachs-or-OA-or-Turnstile)

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

```c++
#include <vector>
using namespace std;

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

**Tips**: 

**Leetcode Link**: https://leetcode.com/discuss/interview-question/699973/Goldman-Sachs-or-OA-or-Turnstile

```C++
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
  vector<int> solve(vector<int> &t, vector<int> &dir) {
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

**Tips**: HashSet

**Leetcode Link**: https://www.lintcode.com/problem/k-substring-with-k-different-characters/solution
```c++
class Solution {
public:
   
    int KSubstring(string &stringIn, int K) {
        // Write your code here
        int Map[5000];
        memset( Map , 0 , sizeof(Map));
        set<string> s;
        int cnt = 0,res = 0;
        for(int i = 0 ; i < K && i < stringIn.length() ; i++) {
            if(!Map[int(stringIn[i])]) {
                cnt ++ ;
            }
            Map[int(stringIn[i])] ++ ;
        }
        if(cnt == K) {
            s.insert(stringIn.substr(0 , K));
        } 
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

```c++
#include <math.h> 
using namespace std;

int utilizationChecks (int instances, vector<int> averageUtil) {
    for (int i = 0; i <= averageUtil.size(); i++) {
        int item = averageUtil[i];
        if (item < 25 && instances > 1) {
            instances = ceil(instances/2.0);
            i += 10;
        } else if (item > 60 && instances < 200000000) {
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

**Tips**: sort

**Leetcode Link**: https://leetcode.com/problems/reorder-data-in-log-files/submissions/

```c++
bool compare(const string &A, const string &B) {
    string cont1 = A.substr(A.find(' ')+1);
    string cont2 = B.substr(B.find(' ')+1);
    
    return cont1 == cont2 ? A < B : cont1 < cont2;
}

class Solution {
public:
    vector<string> reorderLogFiles(vector<string>& logs) {
        vector<string> ret;
        vector<string> let_log, dig_log;
        
        for (auto str : logs) {
            if (isalpha(str[str.size()-1]))
                let_log.push_back(str);
            else
                dig_log.push_back(str);
        }
        
        sort(let_log.begin(), let_log.end(), compare);
        ret = vector<string>(let_log.begin(), let_log.end());
        
        for (auto str : dig_log) {
            ret.push_back(str);
        }
        
        return ret;
    }
};
```

### [Most Common Word](https://algo.monster/problems/most_common_word)

**Difficulty**: Easy

**Frequency**: High

**Tips**: Hashmap + hashset

**Leetcode Link**: https://leetcode.com/problems/most-common-word

```c++
string mostCommonWord(string paragraph, vector<string>& banned) {

	for(auto&c : paragraph)
		c = ispunct(c) ? ' ' : tolower(c);

	string str, res;
	unordered_map<string, int> freq;
	unordered_set<string> b(banned.begin(), banned.end());
	stringstream ss(paragraph);

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
    return pi*size+pj;
}
```

### [Number of Islands](https://leetcode.com/problems/number-of-islands/)

**Difficulty**: Medium

**Frequency**: Very High

**Tips**: BFS/DFS

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

**Tips**: hashmap

**Leetcode links:** https://leetcode.com/discuss/interview-question/989768/Amazon-or-OA-2020-or-Transaction-logs
```c++
vector<string> get_fraud_id(vector<string>& uid, int threshold) {
    vector<string> result;
    unordered_map <string, int> ump;
    
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

**leetcode link:** https://www.lintcode.com/problem/zombie-in-matrix/

```c++
class Solution {
public:
    /**
     * @param grid: a 2D integer grid
     * @return: an integer
     */
    int zombie(vector<vector<int>> &grid) {
        // write your code here
        queue<pair<int, int>> zombies{};
        
        if (grid.size() == 0 && grid[0].size() == 0)
            return -1;
        
        for(int x = 0; x < grid.size(); x ++) {
            for (int y = 0; y < grid[0].size(); y++) {
                if (grid[x][y] == 1)
                    zombies.push(pair<int, int>(x, y));
            }
        }
        
        int days = 0;
        while (!zombies.empty()) {
            int size = zombies.size();
            days ++;
            while (size--) {
                pair<int, int> old_z = zombies.front();
                zombies.pop(); 
                
                if (old_z.first-1 >= 0 && grid[old_z.first-1][old_z.second] == 0) {
                    zombies.push(pair<int, int>(old_z.first-1, old_z.second));
                    grid[old_z.first-1][old_z.second] = 1;
                }
                
                if (old_z.first+1 < grid.size() && grid[old_z.first+1][old_z.second] == 0) {
                    zombies.push(pair<int, int>(old_z.first+1, old_z.second));
                    grid[old_z.first+1][old_z.second] = 1;
                }
                
                if (old_z.second-1 >= 0 && grid[old_z.first][old_z.second-1] == 0) {
                    zombies.push(pair<int, int>(old_z.first, old_z.second-1));
                    grid[old_z.first][old_z.second-1] = 1;
                }
                
                if (old_z.second+1 < grid[0].size() && grid[old_z.first][old_z.second+1] == 0) {
                    zombies.push(pair<int, int>(old_z.first, old_z.second+1));
                    grid[old_z.first][old_z.second+1] = 1;
                }
                
            }
        }
        
         for(int x = 0; x < grid.size(); x ++) {
            for (int y = 0; y < grid[0].size(); y++) {
                if (grid[x][y] == 0)
                    return -1;
            }
        }
        
        return days > 0 ? days-1 : 0;
    }
};
```

### [Music Pairs](https://algo.monster/problems/pairs_of_songs)

**Difficulty**: Medium

**Frequency**: Very High

**Tips**: Modulous

**leetcode link:** https://leetcode.com/problems/pairs-of-songs-with-total-durations-divisible-by-60/submissions/

```c++
class Solution {
public:
    /**
     * @param musics: the musics
     * @return: calc the number of pair of music
     */
    long long musicPairs(vector<int> &musics) {
        // write your code here
        vector<int> ms(60, 0);
        long long ans{};
        
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
        
        for (auto str : words)
            umap[str] ++;
        
        for (auto it : umap)
            pq.push(make_pair(it.second, it.first));
        
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

```c++
int fiveStarReviews(vector<vector<int>> data, int ratingsThreshold) {
    int result = 0;
    int totalProducts = data.size();
    double reqSum = ratingsThreshold * totalProducts * 1.0 / 100;
    double cSum = 0;
    
    while(cSum < reqSum){
        cSum = 0;
        double maxCon = 0;
        int productItem = -1;
        for (int i = 0; i < data.size(); i++)
        {
            double c = (data[i][0] + 1)* 1.0/ (data[i][1] + 1) - data[i][0] * 1.0/ data[i][1];
            if(maxCon < c){
                maxCon = c;
                productItem = i;
            }
            cSum += data[i][0] * 1.0 / data[i][1];
        }
        
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


### [Amazon Fresh Promotion](https://leetcode.com/discuss/interview-question/1002811/Amazon-or-OA-2021-or-Fresh-Promotion)

**Difficulty**: Medium

**Frequency**: High

**Tips**: 

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

