### Description
```From Leetcode Concurrency tag```

Suppose we have a class:
```C++
public class Foo {
  public void first() { print("first"); }
  public void second() { print("second"); }
  public void third() { print("third"); }
}
```
The same instance of Foo will be passed to three different threads. Thread A will call first(), thread B will call second(), and thread C will call third(). Design a mechanism and modify the program to ensure that second() is executed after first(), and third() is executed after second().

 

Example 1:

Input: [1,2,3]

Output: "firstsecondthird"

Explanation: There are three threads being fired asynchronously. The input [1,2,3] means thread A calls first(), thread B calls second(), and thread C calls third(). "firstsecondthird" is the correct output.

### Solution
***Use Semaphore to signal the order of entrance for each thread.***

### Code
```C++
class Foo {
    sem_t a, b;
public:
    Foo() {
        sem_init(&a, 0, 0);
        sem_init(&b, 0, 0);
    }

    void first(function<void()> printFirst) {
        
        // printFirst() outputs "first". Do not change or remove this line.
        printFirst();
        sem_post(&a);
    }

    void second(function<void()> printSecond) {
        
        // printSecond() outputs "second". Do not change or remove this line.
        sem_wait(&a);
        printSecond();
        sem_post(&b);
    }

    void third(function<void()> printThird) {
        
        // printThird() outputs "third". Do not change or remove this line.
        sem_wait(&b);
        printThird();
    }
};
```