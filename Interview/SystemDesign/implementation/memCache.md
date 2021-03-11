## Description
Implement a memcache which support the following features:

get(curtTime, key). Get the key's value, return 2147483647 if key does not exist.
set(curtTime, key, value, ttl). Set the key-value pair in memcache with a time to live (ttl). The key will be valid from curtTime to curtTime + ttl - 1 and it will be expired after ttl seconds. if ttl is 0, the key lives forever until out of memory.
delete(curtTime, key). Delete the key.
incr(curtTime, key, delta). Increase the key's value by delta return the new value. Return 2147483647 if key does not exist.
decr(curtTime, key, delta). Decrease the key's value by delta return the new value. Return 2147483647 if key does not exist.
It's guaranteed that the input is given with increasingcurtTime.

## Implementation

```c++
class Resource {
public:
    int value, expired;
    Resource() {}
    Resource(int v, int e) {
        value = v;
        expired = e;
    }
};

class Memcache {
private:
    map<int, Resource> client;
public:
    Memcache() {
        // initialize your data structure here.
    }

    int get(int curtTime, int key) {
        // Write your code here
        if (client.find(key) == client.end())
            return INT_MAX;

        Resource res = client[key];
        if (res.expired >= curtTime || res.expired == -1)
            return res.value;
        else
            return INT_MAX;
    }

    void set(int curtTime, int key, int value, int ttl) {
        // Write your code here
        int expired;
        if (ttl == 0)
            expired = -1;
        else
            expired = curtTime + ttl - 1;
        client[key] = Resource(value, expired);
    }

    void del(int curtTime, int key) {
        // Write your code here
        if (client.find(key) == client.end())
            return;

        client.erase(client.find(key));
    }
    
    int incr(int curtTime, int key, int delta) {
        // Write your code here
        if (get(curtTime, key) == INT_MAX)
            return INT_MAX;
        client[key].value += delta;
        return client[key].value;
    }
    
    int decr(int curtTime, int key, int delta) {
        // Write your code here
        if (get(curtTime, key) == INT_MAX)
            return INT_MAX;
        client[key].value -= delta;
        return client[key].value;
    }
};
```