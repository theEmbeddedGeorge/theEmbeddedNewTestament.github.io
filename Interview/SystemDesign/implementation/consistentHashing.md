## Description

A general database method for performing a horizontal shard is to take the id against the total number of database servers n and then to find out which machine it is on. The downside of this approach is that as the data continues to increase, we need to increase the database server. When n is changed to n+1, almost all of the data has to be moved, which is not consistent. In order to reduce the defects caused by this naive's hash method (%n), a new hash algorithm emerges: Consistent Hashing, Consistent Hashing. There are many ways to implement this algorithm. Here we implement a simple Consistent Hashing.

Take id to 360. If there are 3 machines at the beginning, then let 3 machines be responsible for the three parts of 0~119, 120~239, 240~359. Then, how much is the model, check which zone you are in, and which machine to go to.
When the machine changes from n to n+1, we find the largest one from the n intervals, then divide it into two and give half to the n+1th machine.
For example, when changing from 3 to 4, we find the third interval 0~119 is the current largest interval, then we divide 0~119 into 0~59 and 60~119. 0~59 is still given to the first machine, 60~119 to the fourth machine.
Then change from 4 to 5, we find the largest interval is the third interval 120~239, after splitting into two, it becomes 120~179, 180~239.
Suppose all the data is on one machine at the beginning. When adding to the nth machine, what is the distribution of the interval and the corresponding machine number?

## Implementation
Consistent Hashing I
```c++
class Solution {
public:
    /**
     * @param n a positive integer
     * @return n x 3 matrix
     */
    vector<vector<int>> consistentHashing(int n) {
        // Write your code here
        vector<vector<int>> results;
        vector<int> machine = {0, 359, 1};
        results.push_back(machine);

        for (int i = 1; i < n; ++i) {
            int index = 0;
            for (int j = 1; j < i; ++j) {
                if (results[j][1] - results[j][0] + 1 >
                    results[index][1] - results[index][0] + 1)
                    index = j;
            }

            int x = results[index][0];
            int y = results[index][1];
            results[index][1] = (x + y) / 2;
            
            machine[0] = (x + y) / 2 + 1;
            machine[1] = y;
            machine[2] = i + 1;
            results.push_back(machine);
        }

        return results;
    }
};
```

Consistent Hashing II
```c++
class Solution
{
  public:
    int n, k;
    map<int, int> shards;
    set<int> ids;
    // @param n a positive integer
    // @param k a positive integer
    // @return a Solution object
    static Solution create(int n, int k)
    {
        // Write your code here
        Solution solution = Solution();
        solution.n = n;
        solution.k = k;
        return solution;
    }

    // @param machine_id an integer
    // @return a list of shard ids
    vector<int> addMachine(int machine_id)
    {
        // Write your code here
        vector<int> random_nums;
        for (int i = 0; i < k; ++i)
        {
            int index;
            do
            {
                index = rand() % n;
            } while (ids.find(index) != ids.end());
            ids.insert(index);
            random_nums.push_back(index);
            shards[index] = machine_id;
        }

        sort(random_nums.begin(), random_nums.end());
        return random_nums;
    }

    // @param hashcode an integer
    // @return a machine id
    int getMachineIdByHashCode(int hashcode)
    {
        // Write your code here
        map<int, int>::iterator it = shards.lower_bound(hashcode);
        if (it == shards.end())
            return shards.begin()->second;
        else
            return it->second;
    }
};
```