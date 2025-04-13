/*
* Filename:     dp-2.c
* Project:      SENG2030 -- A-05
* Programmers:  Curtis Wentzlaff (7274749), Aly Palmer (7382583)
* Date:         April 11th, 2025
* Description:  The logic for the data producer two exists here.
*/

//-------------------------------------------INCLUDES--------------------------------------------//
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


//-------------------------------------------CONSTANTS-------------------------------------------//
static pid_t dp1 = 0;
static int semaphoreID = 0;
static CircularBuffer* buffer;
static char killIt = 0;

//-----------------------------------------PROTOTYPES--------------------------------------------//
int parseArguments(char* argv[], int* shmID);
void sigintHandler(int signal);
char generateRandomData();
int isNumeric(const char* str);

//------------------------------------MAIN FUNCTION----------------------------------------------//
int main(int argc, char* argv[])
{
    int sharedMemoryID;

    if (argc != 2) {
        printf("Usage: %s <shmID>\n", argv[0]);
        return -1;
    }

    if (parseArguments(argv, &sharedMemoryID) < 0) {
        printf("Invalid arguments provided.\n");
        return -1;
    }

    int pid= getpid();

    // Get parent PID (DP-1) using getppid()
    pid_t parentId = getppid();

    // Attach to shared memory
    CircularBuffer* buffer = shmat(sharedMemoryID, NULL, 0);
    if (buffer == (void*)-1) {
        perror("shmat failed in DP-2");
        return -1;
    }

    // Fork to create DC
    int result = fork();
    if (result < 0) {
        perror("fork failed in DP-2");
        shmdt(buffer);
        return -1;
    }
    else if (result == 0) {
        // Child becomes DC
        char shmIDStr[11];
        snprintf(shmIDStr, sizeof(shmIDStr), "%d", sharedMemoryID);
        char parentidString[11];
        snprintf(parentidString, sizeof(parentidString), "%d", parentId);

        // Start DC process
        execl("./../../DC/bin/dc", "dc", shmIDStr, parentidString, NULL);
        perror("execl to dc failed");
        exit(EXIT_FAILURE);
    }

    // DP-2 logic continues
    int semaphoreID = getSemaphore();
    if (semaphoreID < 0) {
        perror("Failed to get semaphore in DP-2");
        shmdt(buffer);
        return -1;
    }

    if (signal(SIGINT, sigintHandler) == SIG_ERR) {
        printf("Error setting up signal for SIGINT.\n");
        shmdt(buffer);
        return -1;
    }

    srand((unsigned int)time(NULL));
    char data;

    while (killIt == 0) {
        data = generateRandomData();

        lockSemaphore(semaphoreID);
        writeBuffer(buffer, data);
        unlockSemaphore(semaphoreID);

        usleep(50000);
    }

    shmdt(buffer);
    return 0;
}

//------------------------------------HELPER FUNCTIONS-------------------------------------------//

// Checks to make sure argument being passed is numeric
int isNumeric(const char* str)
{
    if (str == NULL || *str == '\0')
    {
        return 0;
    }
    while (*str)
    {
        if (!isdigit(*str))
        {
            return 0;
        }

        str++;
    }

    return 1;
}

// Parse command line arguments
int parseArguments(char* argv[], int* shmID)
{
    if (!argv || !shmID)
    {
        fprintf(stderr, "Null pointers passed to parse function.\n");
        return -1;
    }

    if (!isNumeric(argv[1]) || atoi(argv[1]) < 0)
    {
        fprintf(stderr, "Invalid Shared Memory ID.\n");
        return -1;
    }

    *shmID = atoi(argv[1]);

    return 0;
}

// Signal Handler for SIGINT and it sets the flag to kill the process
void sigintHandler(int signal)
{
    killIt = 1;
}

// Randomly generates character 'A' to 'T'
char generateRandomData()
{   
    return 'A' + (rand() % 20);
}