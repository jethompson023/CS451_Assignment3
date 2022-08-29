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
#include <stdbool.h>

char * delayArg;
int totVocalists;
int totComposers;
int totRooms;
int maxWanderTime = -1;
int maxSoundRoomUsageTime = -1;
int randomWanderTime = -1;

sem_t mutex;
//sem_init(&mutex, 0, 1); // will initialize the value of the mutex to 1. You can initialize the mutex to any value of your choice.
//sem_wait(&mutex); // If the mutex value is 1 or more the thread does not block, 
//and it reduces the mutex value by 1; If the mutex value is 0 or below the thread executing this code will
//block until another thread wakes it up executing sem_post
//sem_post(&mutex) // Increase the mutex value by 1 and unblock a thread that is blocked at sem_wait. 

void vocalist_thread_handler(int id) {
    printf("Vocalist %d is Wandering...\n", id);
    sem_wait(&mutex); 

    randomWanderTime = (rand() % (maxWanderTime + 1));
    printf("Task Created with random wander time of %d\n", randomWanderTime);
    int value;
    sem_getvalue(&mutex, &value);
    printf("Mutex value is %d\n", value);
    pthread_exit(0); // this code returns to the corresponding pthread_join issued in main()
} 

void composer_thread_handler(int id) {
    printf("Composer %d is Wandering...\n", id);
    sem_wait(&mutex); 

    randomWanderTime = (rand() % (maxWanderTime + 1));
    printf("Task Created with random wander time of %d\n", randomWanderTime);
    int value;
    sem_getvalue(&mutex, &value);
    printf("Mutex value is %d\n", value);
    pthread_exit(0); // this code returns to the corresponding pthread_join issued in main()
} 

int main(int argc, char* argv[]) {

    if (argc < 2) {
        printf("Necessary arguments are missing, exiting...\n");
        exit(0);
    }
    else {
        delayArg = argv[1];
    }

    if (strcmp(delayArg, "-nodelay") == 0) {
        totVocalists = atoi(argv[2]);
        totComposers = atoi(argv[3]);
        totRooms = atoi(argv[4]);
    }
    else if (strcmp(delayArg, "-randomdelay") == 0) {
        totVocalists = atoi(argv[2]);
        totComposers = atoi(argv[3]);
        totRooms = atoi(argv[4]);
        maxWanderTime = atoi(argv[5]);
        maxSoundRoomUsageTime = atoi(argv[6]);
    }
    else {
        printf("No delay has been specified, exiting...\n");
        exit(0);
    }

    pthread_t vocalist_threads[totVocalists];
    pthread_t composer_threads[totComposers];
    pthread_t room_threads[totRooms];

    sem_init(&mutex, 0, 1);

    printf("Delay argument is %s\n", delayArg);
    printf("Total number of vocalists is %d\n", totVocalists);
    printf("Total number of composers is %d\n", totComposers);
    printf("Total number of rooms is %d\n", totRooms);
    printf("Max wander time is %d\n", maxWanderTime);
    printf("Max sound room usage time is %d\n", maxSoundRoomUsageTime);

    // The following code creates vocalists threads.
    for (int i =0; i < totVocalists; i++)
        pthread_create(&vocalist_threads [i], NULL, vocalist_thread_handler, (void *) i);
    // The following code creates composer threads.
    for (int i =0; i < totComposers; i++)
        pthread_create(&composer_threads [i], NULL, composer_thread_handler, (void *) i);
    // The following code makes sure the main program waits until all threads have finished execution
    for (int i =0; i < totVocalists; i++)
        pthread_join(vocalist_threads[i], NULL);

}

