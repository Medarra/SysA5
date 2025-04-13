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

int writeToSHM(CircularBuffer* shmBuffer, int semID, char* buffer);
void sigintHandler(int signal);

int main(void)
{   
    int shmID;

    // Try to get shared memory
    if ((shmID = shmget(SEM_KEY, sizeof(CircularBuffer), 0)) == -1) {
        // If it doesn't exist, create it
        shmID = shmget(SEM_KEY, sizeof(CircularBuffer), (IPC_CREAT | 0660));
        if (shmID == -1) {
            perror("Failed to create shared memory");
            return -1;
        }
    }

    // Attach to shared memory
    CircularBuffer* shmBuffer = shmat(shmID, NULL, 0);
    if (shmBuffer == (void*)-1) {
        perror("Shmat failed in DP-1");
        return -1;
    }

    int pid= getpid();
    printf("DP-1: %d\n", pid);

    // Fork to create DP-2
    int result = fork();
    if (result < 0) {
        perror("Fork failed");
        shmdt(shmBuffer);
        return -1;
    }
    else if (result == 0) {
        // This is the child — become DP-2
        char idString[11];
        snprintf(idString, sizeof(idString), "%d", shmID);
        execl("./../../DP-2/bin/dp-2", "dp-2", idString, NULL);
        // If execl fails:
        perror("execl to dp-2 failed");
        exit(EXIT_FAILURE);
    }

    // Parent continues as DP-1
    printf("We made it. PID:%d\n", pid);
    int semID = getSemaphore();
    if (semID < 0) {
        perror("Failed to get semaphore");
        shmdt(shmBuffer);
        return -1;
    }

    if (signal(SIGINT, sigintHandler) == SIG_ERR) {
        printf("Error setting up signal for SIGINT.\n");
        shmdt(shmBuffer);
        return -1;
    }

    char buffer[20];
    while (endProgram == 0) {
        for (int i = 0; i < 20; i++) {
            buffer[i] = (rand() % 20) + 65;
        }

        if (writeToSHM(shmBuffer, semID, buffer) < 0) {
            printf("Error writing to shared memory\n");
            break;
        }

        sleep(2);
    }

    // Clean up
    shmdt(shmBuffer);
    return 0;
}

int writeToSHM(CircularBuffer* shmBuffer, int semID, char* buffer) {
    if (lockSemaphore(semID) < 0) {
        return -1;
    }

    int result = 0;
    for (int i = 0; i < 20; i++) {
        result = writeBuffer(shmBuffer, buffer[i]);
        if (result < 0) {
            break;
        }
    }

    if (unlockSemaphore(semID) < 0) {
        result = -1;
    }

    return result;
}

void sigintHandler(int signal) {
    endProgram = 1;
}