## List of Common Brain Teaser Questions

1. Array Game
2. Prefix Hierarchy
3. The Largest String


## Questions and Answer

### **Array Game**
***Questions:***
```
Given an array of integers, determine the number of moves to make all elements equal. Each move consists of choosing all but 1 element and incrementing their values by 1.


Example

numbers = [3, 4, 6, 6, 3]
```

***Solutions:***
```
Concepts covered: Ad-hoc algorithm, array traversal

Optimal Solution: 

Because our goal is to make all elements the same, instead of choosing all but 1 element and incrementing their values by 1, we can choose 1 element and decrease its value by 1 while keeping the rest of the elements the same. Under such conditions (when only the chosen element gets decreased) the only element that shouldn't be decreased is the minimum element of the array. All other elements have to be decreased until they all will be equal to the minimal element. Therefore the answer can be calculated as the sum of differences between each element of the array and the minimum element of the array.
```

### **Prefix Hierarchy**

***Questions:***
```
Given a list of names, determine the number of names in that list for which a given query string is a prefix. The prefix must be at least 1 character less than the entire name string.

Example

names = ['jackson', 'jacques', 'jack']

query = ['jack']

The complete query string 'jack' is a prefix of jackson but not of jacques or jack. The prefix cannot contain the entire name string, so 'jack' does not qualify.

Function Description 

Complete the function findCompletePrefixes in the editor below. The function must return an array of integers that each denotes the number of names strings for which a query string is a prefix.


findCompletePrefixes has the following parameter(s):

    string names[n]:  an array of name strings

    string query[q]:  an array of query strings
```

***Solutions:***
```
Concepts covered: Ad-hoc algorithm, array traversal

Optimal Solution: 

Because our goal is to make all elements the same, instead of choosing all but 1 element and incrementing their values by 1, we can choose 1 element and decrease its value by 1 while keeping the rest of the elements the same. Under such conditions (when only the chosen element gets decreased) the only element that shouldn't be decreased is the minimum element of the array. All other elements have to be decreased until they all will be equal to the minimal element. Therefore the answer can be calculated as the sum of differences between each element of the array and the minimum element of the array.
```