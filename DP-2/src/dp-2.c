int main(int argc, char* argv[])
{
    // Parse command line argument for shmID

    // Grab the PID of itself and DP-1 (its parent)
    // Launch DC application using a fork passing PID for DP-1, DP-2 and shmID

    // Attach to shared memory

    // Check for existance of a semaphore
    // If it doesn't exist, create the semaphore
    // Do we need two semaphores? One for buffer, one for write index?
    
    // Set up signal handler to respond to (or listen for) SIGINT signal

    // While running loop (something regarding the sigint signal)
    //      Generate letter (A to T), and write to buffer
    //      Sleep for 1/20 of a second

    // Detach from shared memory and exit
}