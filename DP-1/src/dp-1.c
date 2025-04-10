int main(void)
{   
    // Check for existence of shared memory
    // If not found, create it
    // Allocates Shared Memory for 256 Character Shared Buffer
    // As Well as the Read Index and Write Index (head and tail)

    //  Attach to shared memory
    // Initialize shared memory elements

    // Through fork call, launch DP-2 (with shmID as a passed command line argument)

    // Check for existance of a semaphore
    // If it doesn't exist, create the semaphore
    // Do we need two semaphores? One for buffer, one for write index?
    
    // Set up signal handler to respond to (or listen for) SIGINT signal

    // While running loop (something regarding the sigint signal)
    //      Loop 20 times or until  buffer is full
    //              generate letter (A to T) and write to sharedMemoryBuffer
    //      Sleep for two seconds

    // Detach from shared memory and exit
}