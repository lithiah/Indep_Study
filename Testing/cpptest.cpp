/*
 * sumodd3.cpp
 *
 * Final version of sum of odd values between 1 and max.
 * Shows how to pass a set of values to a thread function.
 */

#include <pthread.h>
#include <unistd.h>
#include <iostream>

using namespace std;

struct storestuff {
   int half;              // which half the process is computing
   int sum;               // sum of that half
   int max;               // max value
};

void * add_values(void *);

int main()
  {
     pthread_t thread1, thread2;   // two threads
     int half1, half2;             // identify which half of list each
                                   // thread will compute
     int final_sum;                // final sum of both threads
     struct storestuff * ptr1;     // create two structs, one for each thread
     struct storestuff * ptr2;

     cout << "Enter max: ";
     ptr1= new storestuff;
     ptr2= new storestuff;
     cin >> ptr1->max;
     ptr2->max = ptr1->max;

     ptr1->half = 1;
     pthread_create( &thread1, NULL, add_values, (void *) ptr1);

     ptr2->half = 2;
     pthread_create(&thread2, NULL, add_values, (void *) ptr2);

     // must wait for children to finish before displaying sum!
     pthread_join (thread1, NULL);  // wait for thread1 to finish
     cout << "sum from thread1: " << ptr1->sum << endl;
     final_sum = ptr1->sum;

     pthread_join (thread2, NULL);  // wait for thread2 to finish
     cout << "sum from thread2: " << ptr2->sum << endl;
     final_sum += ptr2->sum;

     cout << "Sum: " << final_sum <<  endl;
     return 0;
  }  // main


// separate threads!
void * add_values(void * ptr)
  {
     // passing the pointer makes this pass by reference

     // grab the void pointer and recast it back to struct storestuff
     struct storestuff * local_ptr = (struct storestuff *) ptr;

     // pull out the useful info so that I don't have to keep using pointers
     int half = local_ptr->half;
     int max = local_ptr->max;
     int mymin, mymax;


     // keep the pointer for the sum because this will be passed back
     local_ptr->sum = 0;

     // find min and max for this thread
     if (half == 1) {
        mymin = 1;
        mymax = max / 2;
     } else {
        mymin = max / 2 + 1;
        if (!(mymin % 2))
           mymin++;
        mymax = max;
     }

     while (mymin <= mymax) {
        local_ptr->sum += mymin;
        mymin += 2;
     }
     // the pointer is returned to the calling function
  }
