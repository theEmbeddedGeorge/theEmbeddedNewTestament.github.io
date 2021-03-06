### Approach one: Hash 
```c++
class Solution {
public:
    void count_bits(int arr[], int val) {
        for (int i = 0; i < 32; i++) {
            if (val & (0x1 << i))
                arr[i] ++;
        }
    }
    
    int singleNumberII(vector<int>& nums) {
        int arr[32] = {};
        int ret = 0;
        
        for (auto num : nums) {
            count_bits(arr, num);
        }
        
        for (int i = 0; i < 32; i++) {
            arr[i] = arr[i]%3;
            ret |= (arr[i] << i);
        }
        
        return ret;
    }
};
```

### Approach two: bitwise operation
```c
int getSingle(int arr[], int n)
{
    int ones = 0, twos = 0;
 
    int common_bit_mask;
 
    // Let us take the example of {3, 3, 2, 3} to understand this
    for (int i = 0; i < n; i++) {
        /* The expression "one & arr[i]" gives the bits that are
           there in both 'ones' and new element from arr[].  We
           add these bits to 'twos' using bitwise OR
 
           Value of 'twos' will be set as 0, 3, 3 and 1 after 1st,
           2nd, 3rd and 4th iterations respectively */
        twos = twos | (ones & arr[i]);
 
        /* XOR the new bits with previous 'ones' to get all bits
           appearing odd number of times
 
           Value of 'ones' will be set as 3, 0, 2 and 3 after 1st,
           2nd, 3rd and 4th iterations respectively */
        ones = ones ^ arr[i];
 
        /* The common bits are those bits which appear third time
           So these bits should not be there in both 'ones' and 'twos'.
           common_bit_mask contains all these bits as 0, so that the bits can 
           be removed from 'ones' and 'twos'   
 
           Value of 'common_bit_mask' will be set as 00, 00, 01 and 10
           after 1st, 2nd, 3rd and 4th iterations respectively */
        common_bit_mask = ~(ones & twos);
 
        /* Remove common bits (the bits that appear third time) from 'ones'
             
           Value of 'ones' will be set as 3, 0, 0 and 2 after 1st,
           2nd, 3rd and 4th iterations respectively */
        ones &= common_bit_mask;
 
        /* Remove common bits (the bits that appear third time) from 'twos'
 
           Value of 'twos' will be set as 0, 3, 1 and 0 after 1st,
           2nd, 3rd and 4th itearations respectively */
        twos &= common_bit_mask;
 
        // uncomment this code to see intermediate values
        // printf (" %d %d n", ones, twos);
    }
 
    return ones;
}
 
int main()
{
    int arr[] = { 3, 3, 2, 3 };
    int n = sizeof(arr) / sizeof(arr[0]);
    printf("The element with single occurrence is %d ",
           getSingle(arr, n));
    return 0;
}
```