/*
* Filename:     semaphore.c
* Project:      SENG2030 -- A-05
* Programmers:  Curtis Wentzlaff (7274749), Aly Palmer (7382583)
* Date:         April 11th, 2025
* Description:  This is where the semaphore functionality is stored.
*/

//-------------------------------------------INCLUDES--------------------------------------------//
#include <sys/sem.h>
#include <ctype.h>

//-------------------------------------------CONSTANTS-------------------------------------------//
#define SEM_KEY 1234

//-------------------------------------------STRUCTS---------------------------------------------//
struct sembuf lockOP = {0, -1, 0}; // Lock the semaphore
struct sembuf unlockOP = {0, 1, 0}; // Unlock the semaphore

//-----------------------------------------PROTOTYPES--------------------------------------------//
int getSemaphore();
int lockSemaphore(int semaphoreID);
int unlockSemaphore(int semaphoreID);

//------------------------------------HELPER FUNCTIONS-------------------------------------------//

// Create a New Semaphore
int getSemaphore()
{
    int semaphoreID = semget(SEM_KEY, 1, 0666 | IPC_CREAT);
    if (semaphoreID == -1)
    {
        return -1;
    }

    // Set it up so it starts as unlocked
    if(semctl(semaphoreID, 0, SETVAL, 1) == -1)
    {
        return -1;
    }

    return semaphoreID;
}

// To Lock the Semaphore
int lockSemaphore(int semaphoreID)
{
    if (semop(semaphoreID, &lockOP, 1) == -1)
    {
        return -1;
    }

    return 0;
}

// Unlock the Semaphore
int unlockSemaphore(int semaphoreID)
{
    if(semop(semaphoreID, &unlockOP, 1) == -1)
    {
        return -1;
    }

    return 0;
}