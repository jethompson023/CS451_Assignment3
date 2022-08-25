/*
Author: Reis Ciaramitaro & Jeremiah Thompson
Assignment Number: 3
Date of Submission: 
Name of this file: main.c
Short Description of contents:

*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>

void thread_task(int i) {
    printf("Task Created\n");
    pthread_exit(0); // this code returns to the corresponding pthread_join issued in main()
} 

int main(int argc, char* argv[]) {
     pthread_t thread_id[5];
    // The following code creates 5 threads.
    for (int i =0; i <5; i++)
        pthread_create(&thread_id [i], NULL, thread_task, (void *) i);
    // The following code makes sure the main program waits until all threads have finished execution
    for (int i =0; i <5; i++)
        pthread_join(thread_id[i], NULL);

}

