#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <time.h>
#include "../../Common/src/buffer.c"
#include "../../Common/src/semaphore.c"

static pid_t dp1 = 0;
static int semaphoreID = 0;
static CircularBuffer* buffer;
static char killIt = 0;

int main(int argc, char* argv[])
{
    int* sharedMemoryID;

    if (argc != 3)
    {
        printf("Not enough arguments were provided.\n");
    }

    if (parseArguments(argv, sharedMemoryID) < 0)
     {
         printf("Invalid arguments provided.\n");
         return -1;
     }

    pid_t parentId = getppid();

    int result;
    if (result == 0)
    {
        char sharedMemoryString[11];
        char parentIdString[11];

        snprintf(sharedMemoryString, sizeof(sharedMemoryString), "%d", sharedMemoryID);
        snprintf(parentIdString, sizeof(parentIdString), "%d", parentId);

        execl("./../../DC/bin/dc", "dc", sharedMemoryString, parentIdString, NULL);
    }
    
    buffer = shmat(sharedMemoryID, NULL, 0);

    semaphoreID = getSemaphore();

    if (signal(SIGINT, sigintHandler) == SIG_ERR)
    {
        printf("Error setting up signal for SIGINT.\n");
        return -1;
    }

    srand((unsigned int)time(NULL));
    char data;

    while (killIt == 0)
    {
        data = generateRandomData();

        lockSemaphore(semaphoreID);
        writeBuffer(buffer, data);
        unlockSemaphore(semaphoreID);

        sleep(0.05);
    }

    shmctl(sharedMemoryID, IPC_RMID, NULL);
    
    return 0;
}

int parseArguments(char* argv[], int* shmID)
{
    if (!argv || !shmID || !dp1)
    {
        fprintf(stderr, "Null pointers passed to parse function.\n");
        return -1;
    }

    if (!isNumeric(argv[1]) || atoi(argv[1] < 0))
    {
        fprintf(stderr, "Invalid Shared Memory ID. \n");
        return -1;
    }

    if (!isNumeric(argv[2]) || atoi(argv[2]) <= 0)
    {
        fprintf(stderr, "Invalid DP-1 PID.\n");
        return -1;
    }

    *shmID = atoi(argv[1]);
    dp1 = atoi(argv[2]);

    return 0;
}

void sigintHandler(int signal)
{
    killIt = 1;
}

char generateRandomData()
{   
    return 'A' + (rand() % 20);
}