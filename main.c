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
int randomWanderTime = -1;

sem_t mutex;
sem_t vocalists;
sem_t composers;
int waiting = 0;
int waitingComposers = 0;
int currentVocalist;

void vocalist_thread_handler(int id) {
    printf("Vocalist %d: I am Wandering...\n", id);
    if (isRandDelay) {
        randomWanderTime = (rand() % (maxWanderTime + 1));
        sleep(randomWanderTime);
    }
    printf("Vocalist %d: I am ready to make music...\n", id);
    if (isRandDelay) {
        randomWanderTime = (rand() % (maxSoundRoomUsageTime + 1));
        sleep(randomWanderTime);
    }
    sem_wait(&composers);
    sem_wait(&mutex);
    waiting = waiting - 1;
    currentVocalist = id;
    sem_post(&vocalists);
    sem_post(&mutex);
    printf("Vocalist %d: I am leaving.. Bye\n", id);
} 

void composer_thread_handler(int id) {
    printf("Composer %d: I am Wandering...\n", id);
    if (isRandDelay) {
        randomWanderTime = (rand() % (maxWanderTime + 1));
        sleep(randomWanderTime);
    }
    printf("Composer %d: I am ready to make music...\n", id);
    sem_wait(&mutex);
    if (waiting < totRooms) {
        waiting = waiting + 1;
        sem_post(&composers);
        printf("Composer %d found Vocalist %d\n", id, currentVocalist);
        if (isRandDelay) {
            randomWanderTime = (rand() % (maxSoundRoomUsageTime + 1));
            sleep(randomWanderTime);
        }
        sem_post(&mutex);
        sem_wait(&vocalists);
    }
    sem_post(&mutex);
    printf("Composer %d: I am leaving.. Bye\n", id);
    //randomWanderTime = (rand() % (maxWanderTime + 1));
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

