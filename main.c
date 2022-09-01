/*
    Author: Reis Ciaramitaro & Jeremiah Thompson
    Assignment Number: 3
    Date of Submission: 
    Name of this file: main.c
    Short Description of contents: This program is going to be a simulations of a composer and vocalists
    using a room to make musics. The programs can run in two different modes 1) random delay and 2) no delay.
    Command line input will include randomdelay for the random delay functionality and the nodelay as well.
    There are two threads the composer thread and the vocalist thread both wait for the random time to be within
    0 to the maximum time to wander, time to play music and time to use the soundroom. As this is happening
    the user will see the activity within the terminal. 
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

sem_t rooms;
sem_t vocalists;
sem_t composers;
int waiting = 0;
int waitingComposers = 0;
int currentComposer = -1;
int currentVocalist = -1;
int pair = 0;

/*
    Function Name: vocalist_thread_handler
    Input to the method: id
    Output(Return value): none
    Brief description of the task: This method it handles a vocalist thread using a random delay if instructed too
    Once a composer is found, enter the room then waits for composer to leave the room and then follows.
*/
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

/*
    Function Name: composer_thread_handler
    Input to the method: id
    Output(Return value): none
    Brief description of the task:
    This is method is simlar to the "vocalist_thread_handler" as it handles the composer thread using a random delay if instructed too.
    When the composer is ready it waits for the vocalists to be ready. When the vocalist is ready both have to wait for a room to be open.
    Once a room is found the composer and vocalists begin to make music in the room. Once the random delay is finished the composer will
    leave the room and notify the vocalists. Lastly the room will be opened back up for later use.
*/
void composer_thread_handler(int id) {
    printf("Composer %d: I am Wandering...\n", id);
    
    if (isRandDelay) {
        int randomWanderTime = (rand() % (maxWanderTime + 1));
        sleep(randomWanderTime);
    }
    printf("Composer %d: I am ready to make music...\n", id);

    while (true){
        sem_post(&composers); //tell vocalists a composer is found
        sem_wait(&vocalists); //wait until a vocalist is found
        sem_wait(&rooms); //wait for open rooms
        currentComposer = id;
        
        printf("Composer %d and Vocalist %d found a sound proof room and are making music\n", id, currentVocalist);

        if (isRandDelay) {
            int randomUsageTime = (rand() % (maxSoundRoomUsageTime + 1));
            sleep(randomUsageTime);
        }
        //Remainder section
        printf("Composer %d: I am leaving.. Bye\n", id);
        sem_post(&composers); //vocalists can now leave once composer has
        sem_post(&rooms); //open room back up
        pthread_exit(0);
    }
} 

/*
    Function Name: checkArgs
    Input to the method: all command line arguments
    Output(Return value): none
    Brief description of the task: This method handles all of the commmand line arguments
    and sets them equal to their respective variables.
*/
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

/*
    Function Name: main
    Input to the method: user arguments 
    Output(Return value): activities during the composer and vocalists time making music
    Brief description of the task: This method starts by first checking the variables being entered into this program. After that both
    the composer and vocalist array of threads are initialized to be created later on.
*/
int main(int argc, char* argv[]) {

    checkArgs(argc, argv); //check arguments and set appropriate variables

    pthread_t vocalist_threads[totVocalists];
    pthread_t composer_threads[totComposers];
    pthread_t room_threads[totRooms];

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

