## Problems

1. Insert Delete GetRandom O(1)                
2. LRU Cache
3. Design Add and Search Words Data Structure
4. Find Median from Data Stream                


## Implementation

### **Find Median from Data Stream**

***Big O:*** O(log(n)) speed, O(n) space
```
Tips: 

Two heaps.
```
```c++
class MedianFinder {
    priority_queue<int> lo;
    priority_queue<int> hi;
    
public:
    // Adds a number into the data structure.
    void addNum(int num)
    {
        lo.push(num);                                    // Add to max heap

        hi.push(-lo.top());                               // balancing step
        lo.pop();

        if (lo.size() < hi.size()) {                     // maintain size property
            lo.push(-hi.top());
            hi.pop();
        }
    }

    // Returns the median of current data stream
    double findMedian()
    {
        return lo.size() > hi.size() ? lo.top() : ((double) lo.top() - hi.top()) * 0.5;
    }
};
```