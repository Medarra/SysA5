#define MAX_BUFFER 256

typedef struct
{
    char* const buffer;
    int head;
    int tail;
    int maxBuffer;
}CircularBuffer;

void initBuffer(CircularBuffer* circular, char* buffer)
{
    circular->buffer = buffer;
    circular->head = 0;
    circular->tail = 0;
    circular->maxBuffer = MAX_BUFFER;
}

int isFull(CircularBuffer* circular)
{
    return((circular->head + 1) % circular->maxBuffer) == circular->tail;
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
    circular->head = (circular->head + 1) % circular->maxBuffer;
    return 0;
}

char readBuffer(CircularBuffer* circular)
{
    if (isEmpty(circular))
    {
        return '\0';
    }

    char data = circular->buffer[circular->tail];
    circular->tail = (circular->tail + 1) % circular->maxBuffer;
}
