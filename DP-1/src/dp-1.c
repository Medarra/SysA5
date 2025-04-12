/*
*/

#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

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

    // Check for existance of a semaphore
    // If it doesn't exist, create the semaphore
    // Do we need two semaphores? One for buffer, one for write index?

    // Through fork call, launch DP-2 (with shmID as a passed command line argument)
    
    // Set up signal handler to respond to (or listen for) SIGINT signal

    // While running loop (something regarding the sigint signal)
    //      Loop 20 times or until  buffer is full
    //              generate letter (A to T) and write to sharedMemoryBuffer
    //      Sleep for two seconds

    // Detach from shared memory and exit
}