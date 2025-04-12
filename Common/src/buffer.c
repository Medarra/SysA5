/*
* Filename:     buffer.c
* Project:      SENG2030 -- A-05
* Programmers:  Curtis Wentzlaff (7274749), Aly Palmer (7382583)
* Date:         April 11th, 2025
* Description:  This is where the buffer structure and functionality is stored.
*/

//-------------------------------------------CONSTANTS-------------------------------------------//
#define MAX_BUFFER 256

//-------------------------------------------STRUCTS---------------------------------------------//
typedef struct
{
    char buffer[MAX_BUFFER];
    int write;
    int read;
} CircularBuffer;

//-----------------------------------------PROTOTYPES--------------------------------------------//
int isFull(CircularBuffer* circular);
int isEmpty(CircularBuffer* circular);
int writeBuffer(CircularBuffer* circular, char data);
char readBuffer(CircularBuffer* circular);

//------------------------------------HELPER FUNCTIONS-------------------------------------------//

// Checks if the Buffer is Full
int isFull(CircularBuffer* circular)
{
    return((circular->write + 1) % MAX_BUFFER) == circular->read;
}

// Checks if the Buffer is Empty
int isEmpty(CircularBuffer* circular)
{
    return circular->write == circular->read;
}

// Writes data to the Buffer
int writeBuffer(CircularBuffer* circular, char data)
{
    if (isFull(circular))
    {
        return -1;
    }

    circular->buffer[circular->write] = data;
    circular->write = (circular->write + 1) % MAX_BUFFER;
    return 0;
}

// Reads data from the Buffer, and get rid of it
char readBuffer(CircularBuffer* circular)
{
    if (isEmpty(circular))
    {
        return '\0';
    }

    char data = circular->buffer[circular->read];
    circular->read = (circular->read + 1) % MAX_BUFFER;
}