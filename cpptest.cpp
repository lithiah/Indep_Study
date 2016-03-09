/*
 * threads1.cpp
 *
 * Purpose: Display "hello world" message using separate threads
 * Input  : None
 * Output : "Hello World" ... or is it?
 *
 */

#include <pthread.h>
#include <unistd.h>
#include <iostream>

using namespace std;

void* print_message_function( void *ptr );

int main()
{
   pthread_t thread1, thread2;
   char message1 [6] = "Hello";
   char message2 [6] = "World";

   cout << "This process: " << getpid() << endl;

   pthread_create(&thread1, NULL,
                    print_message_function, (void*) message1);
   cout << "Thread 1: " << thread1 << endl;
   pthread_create(&thread2, NULL,
                    print_message_function, (void*) message2);
   cout << "Thread 2: " << thread2 << endl;



   return 0;
}

void* print_message_function( void *ptr )
{
   char* message;
   message = (char*) ptr;

   *message = 'a';

   cout << message << endl;
/*
   cout << message << "  (PID = " << getpid() << ", PPID = "
        << getppid() << " thread id = " << pthread_self() << ")" << endl;
*/
}
