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

void thread_task(int i) {
    printf("Task Created\n");
    pthread_exit(0); // this code returns to the corresponding pthread_join issued in main()
} 

int main(int argc, char* argv[]) {
    pthread_t thread_id[5];
    char * delayArg;
    int totVocalists;
    int totComposers;
    int totRooms;
    int maxWanderTime = -1;
    int maxSoundRoomUsageTime = -1;

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

    printf("Delay argument is %s\n", delayArg);
    printf("Total number of vocalists is %d\n", totVocalists);
    printf("Total number of composers is %d\n", totComposers);
    printf("Total number of rooms is %d\n", totRooms);
    printf("Max wander time is %d\n", maxWanderTime);
    printf("Max sound room usage time is %d\n", maxSoundRoomUsageTime);

    // The following code creates 5 threads.
    for (int i =0; i < 5; i++)
        pthread_create(&thread_id [i], NULL, thread_task, (void *) i);
    // The following code makes sure the main program waits until all threads have finished execution
    for (int i =0; i < 5; i++)
        pthread_join(thread_id[i], NULL);

}

