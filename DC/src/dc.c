#include <math.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/shm.h>
#include "../../Common/src/common.c"


#define BUFFER_READ_MAX 60 // This very likely changes
#define CHART_FORMAT_PREFIX "%c-%03d "
static char symbols[3] = {'-', '+', '*'};
static char killIt = 0;

typedef struct data
{
    char letter;
    int count;
}data;


int main(int argc, char* argv[])
{
    // int* sharedMemoryID;
    // pid_t* dp1;
    // pid_t* dp2;

    // if (parseArguments(argv, sharedMemoryID, dp1, dp2) < 0)
    // {
        // printf("Invalid arguments provided.\n");
        // return -1;
    // }

    data test[20] = {{'A', 27}, {'B', 134}, {'C', 6}, {'A', 27}, {'B', 134}, {'C', 6}, {'A', 27}, {'B', 134}, {'C', 6}, {'A', 27}, {'B', 134}, {'C', 6}, {'A', 27}, {'B', 134}, {'C', 6}, {'A', 27}, {'B', 134}, {'C', 6}, {'A', 27}, {'B', 134}};
    makeHistogram(test);
    // Parse and store shID, DP-1s PID and DP-2's PID
    // Attach to shared memory

    // Check for semaphore
    // Set up an array of hashvalues to hold the characters and the respective count

    // Set up signal handler for sigint

    // Process Loop (wait for sigint)
    //   NOTE: This is my thought process initially reading this
    //   Initialize a sigalarm count (This is probably a static variable)
    //   set up signalhandler for sigalrm for a function that will
    //   wake up and read from buffer til set maximum
    //   call alarm(2)

    //   The above bit might need to be set up outside the loop

    //   when sigalarm count is => 5
    //       Call make histogram function
    //       It'll take the array of hashvalues and make a graph according to the values


    // Send sigint signal to both dp processes
    // Read remaining data in the buffer
    // Clear screan and display histogram

    // Detach and remove sharedMemory and Semaphore
    // print "Shazam !!"
    // return
}

int parseArguments(char* argv[], int* shmID, pid_t* dp1, pid_t* dp2)
{
    // Parse Shared Memory

    // Parse dp-1 PID

    // Parse dp-2 PID
}

int makeHistogram(data dataCount[])
{

    for (int i = 0; i < 20; i++)
    {
        printf(CHART_FORMAT_PREFIX, dataCount[i].letter, dataCount[i].count);
        int count = dataCount[i].count;

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
}

int readDataHandler(CircularBuffer* buffer)
{
    int currentReadIndex = buffer->head;
    int currentWriteIndex = buffer->tail;

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

    for (int i = 0; i >= toBeRead; i++)
    {
        readBuffer(buffer);
    }

    alarm(2);
}

void sigintHandler(pid_t dp1, pid_t dp2)
{
    kill(dp1, SIGINT);
    kill(dp2, SIGINT);

    killIt = 1;
}