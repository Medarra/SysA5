#include <sys/sem.h>

#define MAX_BUFFER 256
#define SEM_KEY 1234

typedef struct
{
    char buffer[MAX_BUFFER];
    int head;
    int tail;
} CircularBuffer;

struct sembuf lockOP = {0, -1, 0}; // Lock the semaphore
struct sembuf unlockOP = {0, 1, 0}; // Unlock the semaphore

int isFull(CircularBuffer* circular)
{
    return((circular->head + 1) % MAX_BUFFER) == circular->tail;
}

int isEmpty(CircularBuffer* circular)
{
    return circular->head == circular->tail;
}

int writeBuffer(CircularBuffer* circular, char data)
{
    if (isFull(circular))
    {
        return -1;
    }

    circular->buffer[circular->head] = data;
    circular->head = (circular->head + 1) % MAX_BUFFER;
    return 0;
}

char readBuffer(CircularBuffer* circular)
{
    if (isEmpty(circular))
    {
        return '\0';
    }

    char data = circular->buffer[circular->tail];
    circular->tail = (circular->tail + 1) % MAX_BUFFER;
}

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