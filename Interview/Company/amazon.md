## Amazon experience OA questions 2020-2021
- Largest Item Association	2020	Experienced	SHL	USA
- Five Star Sellers	2020	Experienced	HackerRank	USA/Canada
- Beta Testing	2020	Experienced	HackerRank	USA/Canada
- Transaction Logs	2020	Experienced	HackerRank	USA
- Robotics Challenge	2020	Experienced	SHL	USA/UK
- Gifting Groups	2020	Experienced	HackerRank	USA/India

***To-do:***
- [Turnstile](https://leetcode.com/discuss/interview-question/699973/)
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
- [Amazon Fresh Promotion](https://leetcode.com/discuss/interview-question/1002811/Amazon-or-OA-20201-or-Fresh-Promotion)
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
- Prime Air Route
- Amazon Fulfillment Builder	2020	Experienced	HackerRank	USA
- Items in Containers	2020	Experienced	SHL/HackerRank	USA
- Shopping Patterns
- [Substrings of Size K with K-1 Distinct Chars](https://leetcode.com/discuss/interview-question/370112/) (sliding window)
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
  public static int roverMove(String[] cmnds, int n){
	int row=0;
	int col=0;
	for(String cmnd:cmnds) {
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
- Utilization Checks

***Completed:***
- Number of Islands (BFS/DFS)
- Most Common Word  (hashmap)
- Rotting Orange (BFS)
- zombie in matrix (BFS)
- K Closest Points to Origin (Priority Queue/Minheap)
- Top K Frequently elements (Priority Queue/Minheap + hash_map)
- Top K Frequently Mentioned Keywords (Priority Queue/Minheap + hash_map)
- Optimal Utilization (two pointer)
- Two sum unique pair (two pointers + sort)
- Amazon Music Pairs/Pairs of Songs With Total Durations Divisible by 60
- Distance Between Nodes in BST	(DFS in order)

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
