### Description
```From Leetcode Concurrency```
Five silent philosophers sit at a round table with bowls of spaghetti. Forks are placed between each pair of adjacent philosophers.

Each philosopher must alternately think and eat. However, a philosopher can only eat spaghetti when they have both left and right forks. Each fork can be held by only one philosopher and so a philosopher can use the fork only if it is not being used by another philosopher. After an individual philosopher finishes eating, they need to put down both forks so that the forks become available to others. A philosopher can take the fork on their right or the one on their left as they become available, but cannot start eating before getting both forks.

Eating is not limited by the remaining amounts of spaghetti or stomach space; an infinite supply and an infinite demand are assumed.

Design a discipline of behaviour (a concurrent algorithm) such that no philosopher will starve; i.e., each can forever continue to alternate between eating and thinking, assuming that no philosopher can know when others may want to eat or think.



![The problem statement and the image above are taken from wikipedia.org](https://assets.leetcode.com/uploads/2019/09/24/an_illustration_of_the_dining_philosophers_problem.png)

 

The philosophers' ids are numbered from 0 to 4 in a clockwise order. Implement the function void wantsToEat(philosopher, pickLeftFork, pickRightFork, eat, putLeftFork, putRightFork) where:

- philosopher is the id of the philosopher who wants to eat.
- pickLeftFork and pickRightFork are functions you can call to pick the corresponding forks of that philosopher.
- eat is a function you can call to let the philosopher eat once he has picked both forks.
- putLeftFork and putRightFork are functions you can call to put down the corresponding forks of that philosopher.
- The philosophers are assumed to be thinking as long as they are not asking to eat (the function is not being called with their number).
- Five threads, each representing a philosopher, will simultaneously use one object of your class to simulate the process. The function may be called for the same philosopher more than once, even before the last call ends.

### Solution

***My solution is to always pick up left fork and then right fork. However, this may lead to deadlock when all five Philosophers pick the left fork at the same time. My solution is to this issue is to add a semaphore to limit the number of people eating at the same time to be n - 1.***

***Another solution would be always make sure both forks are picked up before eating.***

***See the [wiki link](https://en.wikipedia.org/wiki/Dining_philosophers_problem) for more advanced solutions.*** 

### Code
```C++
#include <semaphore.h>

class DiningPhilosophers {
    sem_t sem_ph;
    mutex mutexes[5];
public:
    DiningPhilosophers() {
        sem_init(&sem_ph, 0, 4);
    }

    void wantsToEat(int philosopher,
                    function<void()> pickLeftFork,
                    function<void()> pickRightFork,
                    function<void()> eat,
                    function<void()> putLeftFork,
                    function<void()> putRightFork) {
        
         // find the left and right fork numbers
        
        int left = philosopher;
        int right = (philosopher + 1) % 5;
        
        sem_wait(&sem_ph);
        {
            // ordered lock to avoid deadlock: fork with min number goes first
            std::lock_guard<std::mutex> first(mutexes[std::min(left, right)]);
            std::lock_guard<std::mutex> second(mutexes[std::max(left, right)]);

            pickLeftFork();
            pickRightFork();

            eat();

            putRightFork();
            putLeftFork();
        }
        sem_post(&sem_ph);
    }
};
```