### Lyft Level 5 Multi stream question

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