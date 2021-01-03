## Hash Table

### Hash Table with Linear Probing
#### Analysis

***Linear Probing***, maybe the most simple one. It solves the Collisions by inserting the value to the next free space after the hashindex the hashfunction gave us. It works great when the values end up on different indexes. When clusters are formed they will decrease the performance dramatically

#### Usage
```
make
./hashTable
```

#### Code
```c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define SIZE 20

struct DataItem {
   int data;   
   int key;
};

struct DataItem* hashArray[SIZE]; 
struct DataItem* dummyItem;
struct DataItem* item;

int hashCode(int key) {
   return key % SIZE;
}

struct DataItem *search(int key) {
   //get the hash 
   int hashIndex = hashCode(key);  
	
   //move in array until an empty 
   while(hashArray[hashIndex] != NULL) {
	
      if(hashArray[hashIndex]->key == key)
         return hashArray[hashIndex]; 
			
      //go to next cell
      ++hashIndex;
		
      //wrap around the table
      hashIndex %= SIZE;
   }        
	
   return NULL;        
}

void insert(int key,int data) {

   struct DataItem *item = (struct DataItem*) malloc(sizeof(struct DataItem));
   item->data = data;  
   item->key = key;

   //get the hash 
   int hashIndex = hashCode(key);

   //move in array until an empty or deleted cell
   while(hashArray[hashIndex] != NULL && hashArray[hashIndex]->key != -1) {
      //go to next cell
      ++hashIndex;
		
      //wrap around the table
      hashIndex %= SIZE;
   }
	
   hashArray[hashIndex] = item;
}

struct DataItem* delete(struct DataItem* item) {
   int key = item->key;

   //get the hash 
   int hashIndex = hashCode(key);

   //move in array until an empty
   while(hashArray[hashIndex] != NULL) {
	
      if(hashArray[hashIndex]->key == key) {
         struct DataItem* temp = hashArray[hashIndex]; 
			
         //assign a dummy item at deleted position
         hashArray[hashIndex] = dummyItem; 
         return temp;
      }
		
      //go to next cell
      ++hashIndex;
		
      //wrap around the table
      hashIndex %= SIZE;
   }      
	
   return NULL;        
}

void display() {
   int i = 0;
	
   for(i = 0; i<SIZE; i++) {
	
      if(hashArray[i] != NULL)
         printf(" (%d,%d)",hashArray[i]->key,hashArray[i]->data);
      else
         printf(" ~~ ");
   }
	
   printf("\n");
}

int main() {
   dummyItem = (struct DataItem*) malloc(sizeof(struct DataItem));
   dummyItem->data = -1;  
   dummyItem->key = -1; 

   insert(1, 20);
   insert(2, 70);
   insert(42, 80);
   insert(4, 25);
   insert(12, 44);
   insert(14, 32);
   insert(17, 11);
   insert(13, 78);
   insert(37, 97);

   display();
   item = search(37);

   if(item != NULL) {
      printf("Element found: %d\n", item->data);
   } else {
      printf("Element not found\n");
   }

   delete(item);
   item = search(37);

   if(item != NULL) {
      printf("Element found: %d\n", item->data);
   } else {
      printf("Element not found\n");
   }
}
```
### Hash Table with Chaining
#### Analysis
***Chaining***. Lastly this way is the most easiest of all. Each Index will be it's own List and so the values with the same hashindexes will be put on the same List. Again, we have a performance increase when many values fall into the same key, but we don't have bad clusters like in Linear Probing. That means that we don't have values from other key's in between, but search only in the specific key List. So, the perfromance will only be affected on those key's that have many values and the others will continue working just fine!

#### Usage
```
make hashTable_chain
./hashTable_chain
```

#### Code
```c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SIZE 20

typedef struct DataItem {
   int data;   
   int key;
   struct DataItem *next;
} DataItem, *pDataItem;

pDataItem hashArray[SIZE]; 
pDataItem dummyItem;
pDataItem item;

int hashCode(int key) {
   return key % SIZE;
}

pDataItem search(int key) {
   //get the hash 
   int hashIndex = hashCode(key);  
   pDataItem dummy;

   dummy = hashArray[hashIndex];
   while(dummy) {
      dummy = hashArray[hashIndex];

      while (dummy) {
        if (dummy->key == key)
            return dummy; 
	      dummy = dummy->next;
      }	
   }        
	
   return NULL;        
}

void insert(int key,int data) {
   pDataItem dummy;
   pDataItem item;

   if (search(key)) {
      printf("Item with the same key %d already exist!\n", key);
      return;
   }
   
   item = (pDataItem) malloc(sizeof(DataItem));
   item->data = data;  
   item->key = key;
   item->next = NULL;

   //get the hash 
   int hashIndex = hashCode(key);
   dummy = hashArray[hashIndex];

   // If it was empty
   if (!dummy) {
       hashArray[hashIndex] = item;
       return;
   }

   //move in array until an empty or deleted cell
   while(dummy && dummy->next) {
      //go to next cell
      dummy = dummy->next;
   }
   dummy->next = item;
}

void delete(int key) {
   pDataItem dummy;
   pDataItem dummy2;

   int hashIndex = hashCode(key);

   dummy = hashArray[hashIndex];

   // if key is at the front
   if (dummy && hashArray[hashIndex]->key == key) {
      dummy = hashArray[hashIndex];
      hashArray[hashIndex] = dummy->next;
      free(dummy);
      return;
   }
   
   // if key is at the middle or end 
   while(dummy && dummy->next) {

      // if key is at the mid
      if(dummy->key == key) {
         dummy2 = dummy->next;
         *dummy = *(dummy->next);
         free(dummy2);
         return;
      }

      // if key is at the end
      if (!dummy->next->next && dummy->next->key == key) {
         dummy2 = dummy->next;
         dummy->next = NULL;
         free(dummy2);
         return;
      }

      dummy = dummy->next;
   }

   // if key is not found
}

void display() {
   int i = 0;
   pDataItem dummy;
	
   printf("===================\n");
   for(i = 0; i<SIZE; i++) {
      dummy = hashArray[i];
      while (dummy) {
         printf(" (%d,%d)",dummy->key,dummy->data);
         dummy = dummy->next;
      }
      
      if (!dummy)
         printf(" ~~ ");
      
      printf("\n");
   }
	
    printf("===================\n");
}

static void check_item(int key) {
   pDataItem item;
   
   item = search(key);

   if(item != NULL) {
      printf("Element found: %d\n", item->data);
   } else {
      printf("Element with key %d not found\n", key);
   }
}

int main() {
   pDataItem item;

   insert(1, 20);
   insert(2, 70);
   insert(42, 80);
   insert(4, 25);
   insert(12, 44);
   insert(14, 32);
   insert(17, 11);
   insert(13, 78);
   insert(37, 97);
   insert(107, 27);
   insert(57, 47);

   // Check hash table and test search
   display();
   check_item(17);
   check_item(37);

   // Test delete and search a non-exist item
   delete(37);
   check_item(37);
   check_item(17);
   display();

   // delete first item 
   insert(77, 438);
   insert(97, 438);
   delete(17);
   display();

   // delete last item
   delete(97);
   display();
   insert(97, 338);
   display();
}
```


#### Reference
https://www.tutorialspoint.com/data_structures_algorithms/hash_table_program_in_c.htm

https://steemit.com/programming/@drifter1/programming-c-hashtables-with-chaining
