/*
*/

#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <signal.h>

#include "../../Common/src/buffer.c"
#include "../../Common/src/semaphore.c"

#define SLEEP_TIME      2           // Number of seconds between letter generation
#define LETTER_NUM      20          // Number of letters sent to shared memory

static int endProgram = 0;

int main(void)
{   
    int shmID; // memory’s ID
    if((shmID = shmget(SEM_KEY,sizeof(CircularBuffer),0)) == -1) {
        // memory doesn’t exist – create it
        shmID = shmget(SEM_KEY, sizeof(CircularBuffer), (IPC_CREAT | 0660));
        if(shmID == -1) {
            //error
            return -1;
        }
    }

    // Check if this process is a fork, changing the process to DP-2 if it is
    int result;             // used for testing the forking process and detaching the semaphore
    if (result == 0) {
        char idString[11] = { '\0' };
        sprintf(idString,"%d", shmID);
        execl("./../DP-2/bin/dp-2", "dp-2", idString, NULL);
    }

    // Attempt to fork the process
    result = fork();
    if (result < 0) {
        //error
        return -1;
    }

    int semID = getSemaphore();
    if (semID < 0) {
        // error
        return -1;
    }
    
    if (signal(SIGINT, sigintHandler) == SIG_ERR) {
        printf("Error setting up signal for SIGINT.\n");
        return -1;
    }

    char buffer[20];
    while (endProgram == 0) {
        for (int i = 0; i < 20; i++) {
            buffer[i] = (rand() % 20) + 65;
        }

        if (writeToSHM(shmID, semID, buffer) < 0) {
            // error
            return -1;
        }

        sleep(2);
    }

    // Detach from shared memory and exit
    shmdt(buffer);
    return 0;
}

int writeToSHM(int shmID, int semID, char* buffer) {
    CircularBuffer* shmBuffer = shmat(shmID, NULL, 0);
    
    int result;
    if ((result = lockSemaphore(semID))) {
        // error
        return -1;
    }

    for (int i = 0; i < 20; i++) {
        if ((result = writeBuffer(shmBuffer, buffer[i])) < 0) {
            break;
        }
    }

    int result;
    if ((result = unlockSemaphore(semID))) {
        // error
        return -1;
    }

    return 0;
}

void sigintHandler(int signal) {
    endProgram = 1;
}