### Lyft Level 5 questions

### Phone screen

**Question: Interrupts are an important part of embedded systems. Consequently, many compiler vendors offer an extension to standard C to support interrupts. Typically, this new key word is __interrupt. The following code uses __interrupt to define an interrupt service routine. Comment on the code.**
```
    __interrupt double compute_area(double radius) {

        double area = PI * radius * radius;
        printf(“nArea = %f”, area);

        return area;
    }
```
**Answer:**

This function has so much wrong with it, it’s almost tough to know 
where to start.

* Interrupt service routines cannot return a value. If you don’t understand this, then you aren’t hired.

* ISR’s cannot be passed parameters. See item (a) for your employment prospects if you missed this.

* On many processors / compilers, floating point operations are not necessarily re-entrant. In some cases one needs to stack additional registers, in other cases, one simply cannot do floating point in an ISR. Furthermore, given that a general rule of thumb is that ISRs should be short and sweet, one wonders about the wisdom of doing floating point math here.

* In a similar vein to point (c), printf() often has problems with reentrancy and performance. If you missed points (c) & (d) then I wouldn’t be too hard on you. Needless to say, if you got these two points, then your employment prospects are looking better and better.

```C++
/*
Implement a function that can be used to control the state of a heater to achieve a desired temperature with 2 degree of hysteresis. The hysteresis is used to prevent excessive toggling of the heater, i.e., if the heater is already on it should stay on until the current temperature reaches the desired temperature + 2, and if the heater is already off then it should stay off until the current temperature reaches the desired temperature - 2.

Example)
Initially, the desired temperature is 70 and the current temperature is 65 so the heater turns on. The current temperature will begin to increase. The heater should remain on until the current temperature reaches 71.

The heater is initially off.

arguments:
setTemp - The set temperature
curTemp - The current temperature

returns:
boolean indicating whether or not the heater should be active
*/

#include <stdint.h>
#include <iostream>

using namespace std;

bool isHeatingRequired(int16_t setTemp, int16_t curTemp, int hyter){

    static bool curr_state = false; // true, 68, 70
    int16_t low_b = (setTemp - hyter) > 0 ? setTemp - hyter : 0;
    int16_t up_b = (setTemp + hyter) <= 0x7FFF ? setTemp + hyter : 0x7FFF;
  
  
    //Write implementation code
    switch(curr_state) { 
      case false: 
        if (curTemp < low_b)
          curr_state = true;
        else
          curr_state = false;
        break;
      case true:
        if (curTemp > up_b)
          curr_state = false;
        else 
          curr_state = true;
        break;
      default:
        break;
    }

    if (curr_state == true)
      std::cout << "Keep heating" << endl;
  
    return curr_state; 
}

void setHeaterState(bool b) {};

int main(){
    int16_t currTemp = 70;
    bool heatingRequired = isHeatingRequired(68, currTemp);
    setHeaterState(heatingRequired);

    return 0;
}
```
## Other questions asked (from multiple sources)
#### Warm up question:
```C++
class Stream:
    def read(self, n: int) -> String:
        ...

You can call read() on the stream and it will return a string with size n. 

It's possible the call will return string shorter than n, in which case the stream of string has been consumed completely.

Example:
s = Stream("Hello World!")
s.read(4)  # => returns "Hell"
s.read(3)  # => returns "o W"
s.read(4)  # => returns "orld"
s.read(5)  # => returns "!" - stream is completely exhausted
s.read(3)  # => returns "" - stream is completely exhausted
```
#### Part 2 Problem:
```c++
Create a new class called MultiStream which can store other streams and has a read() method that is backed by other streams.

Treat Stream objects like black boxes - you cannot change their implementation.

class MultiStream:
    def read(self, n: int) -> String:
        ...
    def add(self, stream: Stream) -> None:
        ...
    def remove(self, stream: Stream) -> None:
        ...

stream1: "ABCDE"
stream2: "12345"

Multisteam ms

ms.add(s1)
ms.add(s2)
ms.read(3)->"ABC" 
ms.read(5)->"DE123"
ms.read(6)->"45"
ms.read(3)->""
s1="ABCDE"
s2="12345"
s3="abcd"
"ABCDE12345abcd"
ms.read(4)->ABCD
ms.remove(s2) 
ms.read(5)->"Eabcd"
Example
ms.read(4)->ABCD
ms.remove(s1) 
ms.read(5)->"12345"
s1="ABCD"
s2="ABCD"
s1s2s3
remove(2)
s1s3
remove(2)
s1

```

### Solution
```C++
#include <iostream>
#include <vector>
using namespace std;

// To execute C++, please define "int main()"
class reader{
    private:
        string s;
        int pos;
    
    public:  
        string read(int n)
        {
            if(pos == s.size())
                return "";

            int i;
            int sz = s.size();
            string ret;

            for(i = pos; i < min(pos+n,sz); i++)
                ret += s[i];
            
            pos = i;

            return ret;
        }

        reader(){}
        reader(string st, int k)
        {
            s = st;
            pos = k;
        }
};


class multiStream{
   private:
        vector<reader> ms;
        vector<bool> visited;

   public:
        string read(int n)
        {
            string ret;

            for(int i = 0; i< ms.size(); i++)
            {
                if(visited[i] == false)
                {
                    ret += ms[i].read(n-ret.size()); 
                    if(ret.size()==n)
                        return ret;
                }
            }
            return ret;
        }
  
        void addStream(string s)
        {
            ms.push_back(reader(s,0));
            visited.push_back(false);
        }
        
        void removeStream(int n)
        {
            if(n > ms.size())
            {
                return; 
            }
            visited[n] = true;
        }
};


int main() {
  // reader obj("Hello World!", 0);
  // cout<<obj.read(4)<<"\n";
  // cout<<obj.read(3)<<"\n";
  // cout<<obj.read(4)<<"\n";
  // cout<<obj.read(5)<<"\n";
  // cout<<obj.read(3)<<"\n";
  multiStream obj;
  obj.addStream("ABCDE");
  obj.addStream("12345");
  obj.addStream("abcde");
  cout<<obj.read(4)<<"\n";
  cout<<obj.read(5)<<"\n";
  obj.removeStream(1);
  cout<<obj.read(5)<<"\n";
  obj.removeStream(2);
  cout<<obj.read(3)<<"\n";
  
  
  return 0;
}
```

### List Version
```C++
class Stream {
    private:
       
    public:
        vector<int> arr;
        int size;
        int pos;
    
        Stream() {}
        Stream(vector<int> new_arr) {
            arr = new_arr;
            size = new_arr.size();
            pos = 0;
        }
    
        int read(vector<int> &ret, int &n) {
            while (n-- > 0 && pos < size) {
                ret.push_back(arr[pos]);
                pos ++;
            }
            return pos == size;
        }
};

int key_hash(vector<int> v) {
    int ret;
    return ret;
}

class multiStream{
   private:
        list<Stream> streams{};
        //unordered_map<Stream, list<Stream>::iterator> umap;
        list<Stream>::iterator curr;
   public:
     multiStream() {
         curr = streams.begin();
     }
    
     vector<int> read(int n)
     {
         vector<int> ret;
         while (curr != streams.end() && n > 0) {
             int incr = curr->read(ret, n);
             if (incr)
                 ++curr;
        }
        for (auto n : ret) {
            cout << n;
        }
         cout << endl;
        return ret;
     }
  
     void addStream(vector<int> s)
     {
         Stream new_stream(s);
         streams.push_back(new_stream);
         if (!streams.empty())
             curr = streams.begin();
     }
     
     void removeStream(int s)
     {
        auto it = streams.begin();
        while (s--) {
            ++ it;
        }
        streams.erase(it);
     }
};


int main() {
  // reader obj("Hello World!", 0);
  // cout<<obj.read(4)<<"\n";
  // cout<<obj.read(3)<<"\n";
  // cout<<obj.read(4)<<"\n";
  // cout<<obj.read(5)<<"\n";
  // cout<<obj.read(3)<<"\n";
  multiStream obj;
  obj.addStream({1,2,3,4,5});
  obj.addStream({6,7,8,9,10});
  obj.addStream({1,2,3});
  vector<int> ret;
  ret = obj.read(4);
    cout << ret.size() << endl;
  ret = obj.read(5);
    cout << ret.size() << endl;
  obj.removeStream(2);
  ret = obj.read(5);
    cout << ret.size() << endl;
  /*ret = obj.read(5); 
    cout << ret.size() << endl;
  ret = obj.read(5);
    cout << ret.size() << endl;
  obj.removeStream(1);
  ret = obj.read(5);
    cout << ret.size() << endl;*/
  //obj.removeStream(2);
  //cout<<obj.read(3)<<"\n";
  
  return 0;
}
```