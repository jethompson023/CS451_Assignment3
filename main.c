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
#include <time.h>


char * delayArg;
bool isRandDelay = false;
int totVocalists;
int totComposers;
int totRooms;
int maxWanderTime = -1;
int maxSoundRoomUsageTime = -1;

sem_t mutex;
sem_t rooms;
sem_t vocalists;
sem_t composers;
int waiting = 0;
int waitingComposers = 0;
int currentComposer = -1;
int currentVocalist = -1;
int pair = 0;

void vocalist_thread_handler(int id) {
    printf("Vocalist %d: I am Wandering...\n", id);
    if (isRandDelay) {
        int randomWanderTime = (rand() % (maxWanderTime + 1));
        sleep(randomWanderTime);
    }
    printf("Vocalist %d: I am ready to make music...\n", id);
    

    while (true) {
        sem_wait(&composers); //wait until a composer is found
    
        currentVocalist = id; //set current vocalist

        sem_post(&vocalists); //composers and vocalists can begin playing once current vocalist is updated
        sem_wait(&composers); //wait until composers and vocalists are done playing
        
        //Remainder section
        printf("Vocalist %d: I am leaving.. Bye\n", id);
        pthread_exit(0);

    }
}

void composer_thread_handler(int id) {
    printf("Composer %d: I am Wandering...\n", id);
    if (isRandDelay) {
        int randomWanderTime = (rand() % (maxWanderTime + 1));
        sleep(randomWanderTime);
    }
    printf("Composer %d: I am ready to make music...\n", id);

    while (true) {

        sem_post(&composers); //tell vocalists a composer is found
        sem_wait(&vocalists); //wait until a vocalist is found
        sem_wait(&rooms); //wait for open rooms
        sem_wait(&mutex); //one at time in the room
        currentComposer = id;
        if (currentVocalist != -1) {
            printf("Composer %d and Vocalist %d found a sound proof room and are making music\n", id, currentVocalist);
            if (isRandDelay) {
                int randomUsageTime = (rand() % (maxSoundRoomUsageTime + 1));
                sleep(randomUsageTime);
            }
            //Remainder section
            printf("Composer %d: I am leaving.. Bye\n", id);
            sem_post(&composers); //vocalists can now leave once composer has
            sem_post(&rooms); //open room back up
            sem_post(&mutex); //composer has left
            pthread_exit(0);
        }
        else {
            sem_post(&rooms); //open room back up
            sem_post(&mutex); //composer has left

        }
    }
} 

void checkArgs(int argc, char* argv[]) {
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
        isRandDelay = true;
    }
    else {
        printf("No delay has been specified, exiting...\n");
        exit(0);
    }
}

int main(int argc, char* argv[]) {

    checkArgs(argc, argv); //check arguments and set appropriate variables

    pthread_t vocalist_threads[totVocalists];
    pthread_t composer_threads[totComposers];
    pthread_t room_threads[totRooms];

    sem_init(&mutex, 0, 1);
    sem_init(&rooms, 0, totRooms);
    sem_init(&vocalists, 0, 0);
    sem_init(&composers, 0, 0);

    // The following code creates vocalists threads.
    for (int i =0; i < totVocalists; i++)
        pthread_create(&vocalist_threads [i], NULL, vocalist_thread_handler, i);
    // The following code creates composer threads.
    for (int i =0; i < totComposers; i++)
        pthread_create(&composer_threads [i], NULL, composer_thread_handler, i);
    // The following code makes sure the main program waits until all threads have finished execution
    for (int i =0; i < totVocalists; i++)
        pthread_join(vocalist_threads[i], NULL);
}

