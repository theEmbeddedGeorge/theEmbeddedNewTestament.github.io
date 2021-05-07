/*
Question 2

A fellow developer has asked you to review the following code. Please
provide your feedback:
*/


#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

//Cong: For comments made by Cong Jin will have a tag: "Cong"*/

/*
* This is a circular buffer that allows enqueue or dequeue arbitrary
* amount of bytes. The buffer size is limited to BUF_SIZE.
*/

static char *GetErrorString(int x);

#define BUF_SIZE    4096 //Cong: can wrap the constant with ()

typedef struct {
   int write_index; //can use uint32_t
   int read_index;  //can use uint32_t
   char *buf;
   int size; //can use size_t or uint32_t
} circular_buf_t;

circular_buf_t buf;

/*
* Enqueue (size) bytes from (client_buf) to the local buf
* memory of (client_buf) is allocated by the caller
*/
int enqueue(char *client_buf, int size)
{
   if (size > BUF_SIZE) {
       printf("%s", GetErrorString(3)); //Should be 2 overflow case
       return 3; //Cong: can #define error code for better logging/debugging
   }

   int part1 = buf.size - buf.write_index;
   memcpy(client_buf, buf.buf + buf.write_index, part1);

   if (part1 != size) { //Cong: condition should be part1 < size, so that we need to continue wrapping the buf and copy the rest
       int part2 = size - part1;//Cong: part2 should take care read_index, since this is where data is still not popped, shoud be size-part1-read_index
       memcpy(client_buf+part1, buf.buf[0], part2);
   }

   return 0;
}

/*
* Dequeue (size) bytes from the local buf to (client_buf),
* (client_buf) is allocated by the caller. It is okay to assume
* (client_buf) is big enough to hold all the (size) bytes
*/
int dequeue(char *client_buf, int size)
{
   if (size < BUF_SIZE) { //Cong: size > BUF_SIZE for overflow condition?
       printf("%s", GetErrorString(2));
       return 2;
   }

   int copy_size = buf.write_index - buf.read_index + 1;
   //Cong: size should be taken int account when assigned actual copy size:
   //copy_size = size <= copy_size ? size : copy_size;                                                  
   memcpy(client_buf, buf.buf + buf.write_index, copy_size); //Cong: should dequeue from buf.buf+buf.read_index

   return 0;
}

//Cong: Personally recommand to put all static funtion at the top
static char *GetErrorString(int x)
{
   char errorString[20]; //Can #define the max error string

   switch ( x )
   {
       case 0:
           errorString = "Success -- No error.";
           break;
       case 2:
           errorString = "Overflow!";
           break;
      //Cong: there is no default case and case 3 handling is not specified
   }

   errorString[19] = 0;//Cong '\0' for terminating the array?
   return errorString;
}


int main(int argc,char* argv[])
{
   // initialize buffer
   buf.buf = malloc(BUF_SIZE); //Cong: 1.better to typecase the return of malloc 2. better to do BUF_SIZE*sizeof(char)
   //Cong: need to check if malloc is returning null pointer due to OOM and do error handling
   buf.size = BUF_SIZE;
   //Cong: Also need to initialize read_index and write_index for intialization

   // Perform enqueue() and dequeue();

   // All completed, return
   return 0;
}