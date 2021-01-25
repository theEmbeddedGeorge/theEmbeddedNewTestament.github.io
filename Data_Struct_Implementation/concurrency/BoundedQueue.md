### Description
```From Leetcode concurrency tag.```

Implement a thread safe bounded blocking queue that has the following methods:

- BoundedBlockingQueue(int capacity) The constructor initializes the queue with a maximum capacity.
- void enqueue(int element) Adds an element to the front of the queue. If the queue is full, the calling thread is blocked until the queue is no longer full.
- int dequeue() Returns the element at the rear of the queue and removes it. If the queue is empty, the calling thread is blocked until the queue is no longer empty.
- int size() Returns the number of elements currently in the queue.

Your implementation will be tested using multiple threads at the same time. Each thread will either be a producer thread that only makes calls to the enqueue method or a consumer thread that only makes calls to the dequeue method. The size method will be called after every test case.

Please do not use built-in implementations of bounded blocking queue as this will not be accepted in an interview.

### Solution

***Use mutex + Semaphore. Mutex to protect queue operation and Semaphore to ensure it does not exceed capacity. Typical Bounded buffer, consumer producer problem.***

### Code
```C++
#include <semaphore.h>

class BoundedBlockingQueue {
    int max_cap;
    queue<int> block_queue;
    mutex mtx;
    sem_t sem_de;
    sem_t sem_en;
public:
    BoundedBlockingQueue(int capacity) {
        max_cap = capacity;
        sem_init(&sem_de, 0, 0);
        sem_init(&sem_en, 0, max_cap);
        block_queue = {};
    }
    
    void enqueue(int element) {
        sem_wait(&sem_en);
        mtx.lock();
        block_queue.push(element);
        mtx.unlock();
        sem_post(&sem_de);
        return;
    }
    
    int dequeue() {
        sem_wait(&sem_de);
        mtx.lock();
        int ret = block_queue.front();
        block_queue.pop();
        mtx.unlock();
        sem_post(&sem_en);
        return ret;
    }
    
    int size() {
        mtx.lock();
        int ret = block_queue.size();
        mtx.unlock();
        return ret;
    }
};
```
