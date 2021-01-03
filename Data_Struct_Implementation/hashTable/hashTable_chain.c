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
