/*
*/

#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

#include "../../Common/src/buffer.c"
#include "../../Common/src/semaphore.c"

#define SLEEP_TIME      2           // Number of seconds between letter generation
#define LETTER_NUM      20          // Number of letter added during letter generation

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

    CircularBuffer* buffer = shmat(shmID, NULL, 0);

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
    }

    // Check for existance of a semaphore
    // If it doesn't exist, create the semaphore
    // Do we need two semaphores? One for buffer, one for write index?
    
    // Set up signal handler to respond to (or listen for) SIGINT signal

    // While running loop (something regarding the sigint signal)
    //      Loop 20 times or until  buffer is full
    //              generate letter (A to T) and write to sharedMemoryBuffer
    //      Sleep for two seconds

    // Detach from shared memory and exit
    shmdt(buffer);
    return 0;
}