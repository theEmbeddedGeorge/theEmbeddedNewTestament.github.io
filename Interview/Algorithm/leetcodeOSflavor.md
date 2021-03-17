## Leetcode problems with OS flavor

1. Read 4 bytes         Easy
2. Read 4 bytes II (call multiple times)    Hard
3. LRU cache    Hard
4. LFU cache    Hard
5. Exclusive Time of Functions  Medium
6. Task Scheduler   Medium



## Implementation 

### Read 4 bytes

### Read 4 bytes II

### LRU cache
Tips: Hashmap + DoubleLinkedList

The problem can be solved with a hashmap that keeps track of the keys and its values in the double linked list. That results in \mathcal{O}(1)O(1) time for put and get operations and allows to remove the first added node in \mathcal{O}(1)O(1) time as well.

```c++
class LRUCache {
    unordered_map<int, int> cache{};
    list<int> table{};
    int cap;

    void move_to_first(int item) {
        table.remove(item);
        table.push_back(item);
    }
    
public:
    LRUCache(int capacity) {
        this->cap = capacity;
    }
    
    int get(int key) {
        if (cache.find(key) == cache.end()) {
            return -1;
        }
        move_to_first(key);
        return cache[key];
    }
    
    void put(int key, int value) {
        if (cache.find(key) != cache.end()) {
            cache[key] = value;
            move_to_first(key);
        } else {
            // if full, evict the first item first
            if (cache.size() == cap) {
                cache.erase(table.front());
                table.erase(table.begin());
            }
            
            move_to_first(key);
            cache[key] = value;
        }
    }
};
```
### LFU cache
![LFU cache design](https://assets.leetcode.com/users/images/4cb3255c-f77a-495d-a3ff-804583a7d5b8_1605533049.7540417.png)
```c++
//Just for better readability
typedef int Key_t;
typedef int Count_t;

struct Node
{
    int value;
    list<Key_t>::iterator itr;
};

class LFUCache
{
    unordered_map<Key_t, Node> m_values;
    unordered_map<Key_t, Count_t> m_counts;
    unordered_map<Count_t, list<Key_t>> m_countKeyMap;
    int m_lowestFrequency;
    int m_maxCapacity;

public:
    LFUCache(int capacity)
    {
        m_maxCapacity = capacity;
        m_lowestFrequency = 0;
    }

    int get(int key)
    {
        if (m_values.find(key) == m_values.end() || m_maxCapacity <= 0)
        {
            return -1;
        }
        //update frequency, & return value
        put(key, m_values[key].value);
        return m_values[key].value;
    }

    void put(int key, int value)
    {
        if (m_maxCapacity <= 0)
        {
            return;
        }

        //If key is not present and capacity has exceeded,
        //then remove the key entry with least frequency
        //else just make the new key entry
        if (m_values.find(key) == m_values.end())
        {
            if (m_values.size() == m_maxCapacity)
            {
                int keyToDelete = m_countKeyMap[m_lowestFrequency].back(); 
                m_countKeyMap[m_lowestFrequency].pop_back();
                if (m_countKeyMap[m_lowestFrequency].empty())
                {
                    m_countKeyMap.erase(m_lowestFrequency);
                }
                m_values.erase(keyToDelete);
                m_counts.erase(keyToDelete);
            }
            m_values[key].value = value;
            m_counts[key] = 0;
            m_lowestFrequency = 0;
            m_countKeyMap[m_counts[key]].push_front(key);
            m_values[key].itr = m_countKeyMap[0].begin();
        }
        //Just update value and frequency
        else
        {
            m_countKeyMap[m_counts[key]].erase(m_values[key].itr);
            if (m_countKeyMap[m_counts[key]].empty())
            {
                if (m_lowestFrequency == m_counts[key])
                    m_lowestFrequency++;
                m_countKeyMap.erase(m_counts[key]);
            }
            m_values[key].value = value;
            m_counts[key]++;
            m_countKeyMap[m_counts[key]].push_front(key);
            m_values[key].itr = m_countKeyMap[m_counts[key]].begin();
        }
    }
};
```
### Exclusive Time of Functions

### Task Scheduler
Two Situations: 
- The most frequent task is not frequent enough to force the presence of idle slots.
- The most frequent task is frequent enough to force some idle slots.

The first situation is straightforward because the total number of slots is defined by the number of tasks: len(tasks)

The second situation is a bit more tricky and requires to know the number n_max and the frequency f_max of the most frequent tasks.

If the number of slots to use is defined by the most frequent task, it's equal to  
    
    (f_max - 1) * (n + 1) + n_max.

```c++
class Solution {
public:
    int leastInterval(vector<char>& tasks, int n) {
        vector<int> count(26, 0);
        int tasks_size = tasks.size();
        int max_freq = 0;
        for (auto &it : tasks) {
            count[it - 'A']++; 
            max_freq = max(max_freq, count[it - 'A']);
        }

        int result = (max_freq - 1) * (n+1);
        for (auto &it : count) {
            if (max_freq == it) {
                result++;
            }
        }

        return max(result, tasks_size);
    }
};
```

