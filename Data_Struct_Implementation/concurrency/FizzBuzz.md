### Description
```From Leetcode Concurrency Tag.```

Write a program that outputs the string representation of numbers from 1 to n, however:

- If the number is divisible by 3, output "fizz".
- If the number is divisible by 5, output "buzz".
- If the number is divisible by both 3 and 5, output "fizzbuzz".

For example, for n = 15, we output: 1, 2, fizz, 4, buzz, fizz, 7, 8, fizz, buzz, 11, fizz, 13, 14, fizzbuzz.

Suppose you are given the following code:
```C++
class FizzBuzz {
  public FizzBuzz(int n) { ... }               // constructor
  public void fizz(printFizz) { ... }          // only output "fizz"
  public void buzz(printBuzz) { ... }          // only output "buzz"
  public void fizzbuzz(printFizzBuzz) { ... }  // only output "fizzbuzz"
  public void number(printNumber) { ... }      // only output the numbers
}
```
Implement a multithreaded version of FizzBuzz with four threads. The same instance of FizzBuzz will be passed to four different threads:

- Thread A will call fizz() to check for divisibility of 3 and outputs fizz.

- Thread B will call buzz() to check for divisibility of 5 and outputs buzz.

- Thread C will call fizzbuzz() to check for divisibility of 3 and 5 and outputs fizzbuzz.

- Thread D will call number() which should only output the numbers.

### Solution

***Use condition variable + mutex***

### Code
```C++
class FizzBuzz {
private:
    int n;
    atomic<int> current;
    mutex mtx;
    condition_variable cv;

protected:
    void do_work(function<void(int)> printN, function<bool()> check) {
        while (current <= n) {
            std::unique_lock<mutex> lk(mtx);
            cv.wait(lk, [&]{return current > n || check();});
                    
            if (current > n) break;
            printN(current);
            current ++;
            cv.notify_all();
        }
    }
    
public:
    FizzBuzz(int n) {
        this->n = n;
        this->current = 1;
    }

    // printFizz() outputs "fizz".
    void fizz(function<void()> printFizz) {
        do_work([&](int i){printFizz();}, [&]{return (current%3 == 0) && (current%5 !=0);});
    }

    // printBuzz() outputs "buzz".
    void buzz(function<void()> printBuzz) {
        do_work([&](int i){printBuzz();}, [&]{return current%3 != 0 && current%5 ==0;});
    }

    // printFizzBuzz() outputs "fizzbuzz".
	void fizzbuzz(function<void()> printFizzBuzz) {
        do_work([&](int i){printFizzBuzz();}, [&]{return current%3 == 0 && current%5 ==0;});
    }

    // printNumber(x) outputs "x", where x is an integer.
    void number(function<void(int)> printNumber) {
        do_work([&](int i){printNumber(i);}, [&]{return current%3 != 0 && current%5 !=0;});
    }
};
```