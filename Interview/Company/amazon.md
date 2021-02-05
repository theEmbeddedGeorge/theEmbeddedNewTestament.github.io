## Amazon experience OA questions 2020-2021

[Blind thread](https://www.teamblind.com/post/amazon-oa-question-complilation-xjQosyo3)

- Robotics Challenge	
- Gifting Groups	

***To-do:***
```C++
#include <algorithm>
#include <unordered_map>
#include <string>
#include <sstream>

using namespace std;

vector<string> get_fraud_id(vector<string>& uid, int threshold) {
    vector<string> result;
    unordered_map <string, int> ump;
    vector<string> temp;
    int idx;
    string word;
    for (idx = 0; idx < uid.size(); idx++) {
        stringstream ss(uid[idx]);
        while (ss >> word) {
            temp.push_back(word);
        }
        if (temp[0] != temp[1]) {
            ump[temp[0]]++;
            ump[temp[1]]++;
        }
        else
            ump[temp[0]]++;
        temp.clear();
    }
    for (auto &i1 : ump) {
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
- [Five Star Sellers](https://algo.monster/problems/five_star_sellers)
- [Beta Testing](https://leetcode.com/problems/minimum-difficulty-of-a-job-schedule/)
- [Largest Item Association](https://leetcode.com/discuss/interview-question/844979/Amazon-or-OA-2020-or-Largest-Item-Association)
- [Turnstile](https://leetcode.com/discuss/interview-question/699973/Goldman-Sachs-or-OA-or-Turnstile)
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
- [Amazon Fresh Promotion](https://leetcode.com/discuss/interview-question/1002811/Amazon-or-OA-2021-or-Fresh-Promotion)
```JAVA
private static int freshPromotion(String[][] codeList, String[] shoppingCart) {
//        Start at 0 index for both the code list and shopping cart.
        int cartIdx = 0, codeIdx = 0;
        while (cartIdx < shoppingCart.length && codeIdx < codeList.length) {
            String cur = shoppingCart[cartIdx];
//            If the first fruit of the codeList is anything or if it matches the current fruit at the cart idx.
            if((codeList[codeIdx][0].equals("anything") || codeList[codeIdx][0].equals(cur)) && hasOrder(shoppingCart, cartIdx, codeList[codeIdx])){
                cartIdx += codeList[codeIdx++].length;
            }else{
                cartIdx++;
            }
        }
//        If the all the codeList is present then return 1, else 0.
        return codeIdx == codeList.length ? 1 : 0;
    }

    private static boolean hasOrder(String[] shoppingCart, int idx, String[] order) {
//        Loop through the codeList to check if the fruits are in order.
        for (String s : order) {
            if (idx < shoppingCart.length && (s.equals("anything") || shoppingCart[idx].equals(s))){
                idx++;
            }else{
                return false;
            }
        }
        return true;
    }
```
- Amazon Fulfillment Builder
- [Items in Containers](https://algo.monster/problems/items_in_containers)
```c++
function numberOfItems(s, startIndices, endIndices) {
    int sum = 0;
    int currCount = 0;
    bool start = false;
    const prefix = s.split('').map((x) => {
    if (!start && x === '|') {
        start = true;
    }
    if (start && x === '|') {
        if (currCount != 0) {
            sum += currCount;
            currCount = 0;
        }
    }
    if (start && x === '*') {
        currCount += 1;
    }
return sum;
```
- [Shopping Patterns](https://algo.monster/problems/shopping_patterns)
- [Substrings of Size K with K-1 Distinct Chars](https://leetcode.com/discuss/interview-question/370112/) (sliding window)
- [Substrings of Size K with K-1 Distinct Chars](https://www.lintcode.com/problem/k-substring-with-k-different-characters/solution)
```JAVA
 public List<String> Count(String S,int k){
        int distinct=0,i=0;
        int [] memo=new int[26];
        Set<String> set=new HashSet<>();
        for (;i<k;i++){
            if (memo[S.charAt(i)-'a']==0)
                distinct+=1;
            memo[S.charAt(i)-'a']+=1;
        }
        if (distinct==k) {
            set.add(S.substring(i-k,i));
        }
        while (i<S.length()){
            if (memo[S.charAt(i)-'a']==0)
                distinct+=1;
            memo[S.charAt(i)-'a']+=1;
            memo[S.charAt(i-k)-'a']-=1;
            if (memo[S.charAt(i-k)-'a']==0)
                distinct-=1;
            if (distinct==k)
                set.add(S.substring(i-k+1,i+1));
            i+=1;
            }

        return new ArrayList<>(set);
    }
```
- [Rover Control](https://leetcode.com/discuss/interview-question/985703/Amazon-or-OA-or-Rover-Control) 
```C++
  static int roverMove(vector<string> cmnds, int n){
	int row=0;
	int col=0;
	for(auto cmnd:cmnds) {
		if(cmnd=="RIGHT") {
			if(col<n-1)col=col+1;
		}
		else if(cmnd=="LEFT") {
			if(col>0)col=col-1;
		}
		else if(cmnd=="UP") {
			if(row>0)row=row-1;
        }
		else {
			if(row<n-1) row=row+1;
		}
	}
	return (row*n)+col;
}
```

***Completed:***
- [Reorder Data in Log Files](https://leetcode.com/problems/reorder-data-in-log-files/submissions/)
- [Transaction Logs](https://aonecode.com/amazon-online-assessment-transaction-logs)
- [Number of Islands (BFS/DFS)](https://leetcode.com/problems/number-of-islands)
- [Most Common Word  (hashmap)](https://leetcode.com/problems/most-common-word)
- [Rotting Orange (BFS)](https://leetcode.com/problems/rotting-oranges)
- [zombie in matrix (BFS)](https://www.lintcode.com/problem/zombie-in-matrix/description)
- [K Closest Points to Origin (Priority Queue/Minheap)](https://leetcode.com/problems/k-closest-points-to-origin)
- [Top K Frequently elements (Priority Queue/Minheap + hash_map)](https://leetcode.com/problems/top-k-frequent-elements)
- [Top K Frequently Mentioned Keywords (Priority Queue/Minheap + hash_map)](https://www.lintcode.com/problem/top-k-frequently-mentioned-keywords/)
- [Optimal Utilization (two pointer)](https://www.lintcode.com/problem/optimalutilization/)
- Two sum unique pair (two pointers + sort)
- [Amazon Music Pairs/Pairs of Songs With Total Durations Divisible by 60](https://leetcode.com/problems/pairs-of-songs-with-total-durations-divisible-by-60)
- [Distance Between Nodes in BST	(DFS in order)](https://leetcode.com/problems/minimum-distance-between-bst-nodes)
- [Prime Air Route/Two sum to the closest (Two Pointer))](https://leetcode.com/discuss/interview-question/1025705/Amazon-or-OA-or-Prime-Air-time)

## Interview process

Amazon Interview:

Round1: LP, DS/ALGO coding problem. [LC medium]
Round2: LP, 2 DS/ALGO coding problem [LC medium]
Round3: All LP, Domain experience
Round4: Embedded Design
Round5: LP, DS/ALGO coding problem [LC medium]
Round6: LP, and work experience, embedded systems

## Reference

https://leetcode.com/discuss/interview-experience/974067/NDA-or-Microsoft-or-Amazon-or-Senior-Software-Engineer-(Embedded-Software)-or-India-or-Offers

[Amazon Online Assessment 2021](https://aonecode.com/amazon-online-assessment-questions)

[Amazon OA collection](https://www.1point3acres.com/bbs/thread-699232-1-1.html)

[Amazon OA leetcode](https://leetcode.com/discuss/interview-question/344650/Amazon-Online-Assessment-Questions)

DFS
BFS
Sliding Window
Tree
LinkedList.

[Prime Air Route]
[Amazon Fresh Promotion](https://leetcode.com/problems/k-closest-points-to-origin/)
[Optimal Utilization](https://leetcode.com/discuss/interview-question/373202/Amazon-or-OA-2019-or-Optimal-Utilization)
[earliest time to complete delivery]
[two sum pairs]
[word break II]
[Merge K sorted list]

[社招OA2题](https://1o24bbs.com/t/topic/3152)

总结一下截止到现在社招 OA 题库：
具体的题地里很多了 大家稍找一下就有了
1. 卡车装M个箱子， N个地点List<Integer> M<N
列出最近的M个位置。 要注意输入不正常的情况，比如只有一个输入
http://www.1point3acres.com/bbs/thread-289277-1-1.html
2. 给个无序数组要构建BST ， 然后找出Node1 Node2 距离 我觉得这道题要好好准备一下，我准备了好久看地理有大佬testcase 没过瑟瑟发抖中
http://www.1point3acres.com/bbs/thread-192414-1-1.html
3. 棒球题目 stack 解决。这个要注意细节反正我花了好久才理解题目的意思
http://www.1point3acres.com/bbs/thread-270278-1-1.html
4. 高尔夫场砍树问题。 PQ + BFS ( LC The Maze II)
http://www.1point3acres.com/bbs/thread-288738-1-1.html
5. common manager 问题 nnary-lca
http://www.1point3acres.com/bbs/thread-288537-1-1.html
6. 找所有anagram http://www.1point3acres.com/bbs/thread-288537-1-1.html
7. 水果清单 就是水果清单上的必须按顺序输出在shoppingcart里面
http://www.1point3acres.com/bbs/thread-288738-1-1.html
8. valid Parentheses LC 原题 stack 解
9. 给个Movie movie BFS 找出所有关联电影中top K rate 的电影 Set<Movie> 这道题我找了好久没有找到比较好的类题，大家将就准备
http://w
10. 菜单就是给一个人名list 然后菜单国家的list，要求人名对应到国家再对应到菜品。 我的做法是建两个hashmap
http://www.1point3acres.com/bbs/thread-280797-1-1.html
11. 最大互联点数集合  itemAssociation
http://www.1point3acres.com/bbs/thread-280797-1-1.html
http://www.1point3acres.com/bbs/thread-281940-1-1.html
12. golf球场修场地。
这个是老题 大家搜一搜就好
13. maze 题目最近很常考
就是给一个maze，0 不能走， 1可以走，走到9 问最小步数。 我的做法是用用bfs + terminate condition
具体题目链接在这里https://www.1point3acres.com/bbs ... 6orderby%3Ddateline
14. anagram 问题leetcode原题
15. k distinct subtring 这个我开始试着用了很多的方法，都输出不对，因为输出长度也是k， 后来用了slidingwindow 然后checks
16. 飞机里程或者capacity 最近， 就是给了两个list 对应编号， 让求出各拿出一个元素的和最接近一个值 我觉得这道题可以用bfs+ priority queue来做，但是没有动手做，地理有大佬建树做的，太大佬了， 我不是很建议用two pointer， 因为two pointers 要来来回回的走， 来加上重复的历程， 我用的bf做的。
具体题目看这个https://1o24bbs.com/t/topic/3374

Prime Air Route	2020	Experienced	HackerRank	USA
Amazon Fulfillment Builder	2020	Experienced	HackerRank	USA
Amazon Fresh Promotion	2020	Experienced	SHL/HackerRank	USA
Amazon Music Pairs	2020	Experienced	HackerRank	USA/India
Items in Containers	2020	Experienced	SHL/HackerRank	USA
Largest Item Association	2020	Experienced	SHL	USA
Turnstile	2020	Experienced	SHL/HackerRank	USA
Five Star Sellers	2020	Experienced	HackerRank	USA/Canada
Beta Testing	2020	Experienced	HackerRank	USA/Canada
Utilization Checks	2020	Experienced	HackerRank	USA/India
Top K Frequently Mentioned Keywords	2020	Experienced	SHL	USA
Transaction Logs	2020	Experienced	HackerRank	USA
Substrings of Size K with K-1 Distinct Chars	2020	Experienced	SHL/HackerRank	USA
Number of Islands	2020	Experienced	SHL/HackerRank	USA
Most Common Word	2020	Experienced	SHL/HackerRank	USA
Rover Control	2020	Experienced	HackerRank	USA
Distance Between Nodes in BST	2020	Experienced	SHL	USA/UK
Robotics Challenge	2020	Experienced	SHL	USA/UK
K Closest Points to Origin	2020	Experienced	SHL	USA
Shopping Patterns	2020	Experienced	SHL/HackerRank	USA/India
