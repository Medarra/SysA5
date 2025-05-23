/*
* Filename:     dc.c
* Project:      SENG2030 -- A-05
* Programmers:  Curtis Wentzlaff (7274749), Aly Palmer (7382583)
* Date:         April 11th, 2025
* Description:  The logic for the data consumer exists in this file,
*               including the creation of histograms.
*/

//-------------------------------------------INCLUDES--------------------------------------------//
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include "../../Common/src/buffer.c"
#include "../../Common/src/semaphore.c"

//-------------------------------------------CONSTANTS-------------------------------------------//
#define BUFFER_READ_MAX 60
#define CHART_FORMAT_PREFIX "%c-%03d "

static char symbols[3] = {'-', '+', '*'};
static char killIt = 0;
static pid_t dp1 = 0;
static pid_t dp2 = 0;
static CircularBuffer* buffer;
static int semaphoreID = 0;
static int readCount = 0;
static int letterCounts[26] = {0};  // A–Z

//-----------------------------------------PROTOTYPES--------------------------------------------//
int isNumeric(const char* str);
int parseArguments(char* argv[], int* shmID);
int makeHistogram();
void readDataHandler(int signal);
void sigintHandler(int signal);

//------------------------------------MAIN FUNCTION----------------------------------------------//
int main(int argc, char* argv[])
{
    int sharedMemoryID;

    if (argc != 3)
    {
        printf("Not enough arguments were provided.\n");
        return -1;
    }

    if (parseArguments(argv, &sharedMemoryID) < 0)
    {
        printf("Invalid arguments provided.\n");
        return -1;
    }

    dp2 = getppid();

    buffer = shmat(sharedMemoryID, NULL, 0);
    if (buffer == (void*)-1)
    {
        perror("shmat");
        return -1;
    }

    semaphoreID = getSemaphore();

    if (signal(SIGINT, sigintHandler) == SIG_ERR)
    {
        printf("Error setting up signal for SIGINT.\n");
        return -1;
    }

    if (signal(SIGALRM, readDataHandler) == SIG_ERR)
    {
        printf("Error setting up signal for SIGALRM.\n");
        return -1;
    }

    alarm(2); // Start periodic read loop

    while (killIt == 0)
    {
        if (readCount >= 5)
        {
            makeHistogram();
            readCount = 0;
        }
    }

    // Final flush of buffer
    char dataRead;
    while (!isEmpty(buffer))
    {
        dataRead = readBuffer(buffer);
        if (dataRead >= 'A' && dataRead <= 'T')
        {
            letterCounts[dataRead - 'A']++;
        }
    }

    system("clear");
    makeHistogram();

    shmctl(sharedMemoryID, IPC_RMID, NULL);
    semctl(semaphoreID, 0, IPC_RMID);

    printf("Shazam !!\n");
    return 0;
}

//------------------------------------HELPER FUNCTIONS-------------------------------------------//

int isNumeric(const char* str)
{
    if (str == NULL || *str == '\0') return 0;

    while (*str)
    {
        if (!isdigit(*str)) return 0;
        str++;
    }

    return 1;
}

int parseArguments(char* argv[], int* shmID)
{
    if (!argv || !shmID)
    {
        fprintf(stderr, "Null pointers passed to parse function.\n");
        return -1;
    }

    if (!isNumeric(argv[1]) || atoi(argv[1]) < 0)
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

int makeHistogram()
{
    for (int i = 0; i < 20; i++)
    {
        printf(CHART_FORMAT_PREFIX, 'A' + i, letterCounts[i]);

        int count = letterCounts[i];
        for (int j = 2; j >= 0; j--)
        {
            while (count >= pow(10, j))
            {
                printf("%c", symbols[j]);
                count -= pow(10, j);
            }
        }

        printf("\n");
    }

    printf("\n");
    return 0;
}

void readDataHandler(int signal)
{
    lockSemaphore(semaphoreID);

    int currentReadIndex = buffer->read;
    int currentWriteIndex = buffer->write;
    int toBeRead = 0;

    if (currentReadIndex <= currentWriteIndex)
    {
        toBeRead = currentWriteIndex - currentReadIndex;
    }
    else
    {
        toBeRead = (MAX_BUFFER - currentReadIndex) + currentWriteIndex;
    }

    if (toBeRead > BUFFER_READ_MAX)
    {
        toBeRead = BUFFER_READ_MAX;
    }

    char dataRead;
    for (int i = 0; i < toBeRead; i++)
    {
        dataRead = readBuffer(buffer);
        if (dataRead >= 'A' && dataRead <= 'Z')
        {
            letterCounts[dataRead - 'A']++;
        }
    }

    unlockSemaphore(semaphoreID);

    readCount++;
    alarm(2);
}

void sigintHandler(int signal)
{
    kill(dp1, SIGINT);
    kill(dp2, SIGINT);
    killIt = 1;
}
