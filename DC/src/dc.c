#define BUFFER_READ_MAX 60 // This very likely changes

typedef struct data
{
    char letter;
    int count;
};


int main(int argc, char* argv[])
{
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