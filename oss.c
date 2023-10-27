#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <time.h>
#include <sys/shm.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/types.h>

// Define the maximum number of child processes
#define MAX_CHILD_PROCESSES 18
// Define the key for shared memory segment for the clock
#define CLOCK_SHARED_MEMORY_KEY 1994
// Define the clock update interval (1 millisecond in nanoseconds)
#define CLOCK_UPDATE_INTERVAL 1000000

// Define the log file
FILE* logFile;

typedef struct {
    unsigned int seconds;
    unsigned int nanoseconds;
} SimulatedClock;

typedef struct {
    int occupied;
    pid_t pid;
    int startSeconds;
    int startNano;
    int serviceTimeSeconds;
    int serviceTimeNano;
    int eventWaitSec;
    int eventWaitNano;
    int blocked;
} PCB;

SimulatedClock* systemClock; // Simulated system clock
PCB processTable[MAX_CHILD_PROCESSES]; // Process Control Blocks

// Function to initialize the simulated system clock
void initializeClock() {
    systemClock->seconds = 0;
    systemClock->nanoseconds = 0;
}

// Function to update the clock
void updateClock() {
    systemClock->nanoseconds += CLOCK_UPDATE_INTERVAL;
    if (systemClock->nanoseconds >= 1000000000) {
        systemClock->seconds++;
        systemClock->nanoseconds -= 1000000000;
    }
}

// Function to log messages
void logMessage(const char* message) {
    time_t rawtime;
    struct tm* timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);

    fprintf(logFile, "OSS: [%02d:%02d:%02d:%03d] %s\n", timeinfo->tm_min, timeinfo->tm_sec, systemClock->seconds, systemClock->nanoseconds / 1000000, message);
    fflush(logFile);
}

int main(int argc, char* argv[]) {
    int n_value = 0;
    int s_value = 0;
    int t_value = 0;
    char* f_value = NULL;
    int opt;

    // Parse command-line arguments
    while ((opt = getopt(argc, argv, "n:s:t:f:")) != -1) {
        switch (opt) {
            case 'n':
                n_value = atoi(optarg);
                break;
            case 's':
                s_value = atoi(optarg);
                break;
            case 't':
                t_value = atoi(optarg);
                break;
            case 'f':
                f_value = optarg;
                break;
            default:
                fprintf(stderr, "Usage: %s -n <value> -s <value> -t <value> -f <filename>\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    // Open the log file for writing
    logFile = fopen(f_value, "w");
    if (logFile == NULL) {
        perror("oss: Failed to open log file");
        exit(EXIT_FAILURE);
    }

    // Create or access the shared memory for the clock
    int shmid;
    if ((shmid = shmget(CLOCK_SHARED_MEMORY_KEY, sizeof(SimulatedClock), IPC_CREAT | 0666)) == -1) {
        perror("oss: Failed to create shared memory segment for clock");
        exit(1);
    }

    // Attach shared memory segment to systemClock
    if ((systemClock = (SimulatedClock*)shmat(shmid, NULL, 0)) == (void*)-1) {
        perror("oss: Failed to attach shared memory segment for clock");
        exit(1);
    }

    // Initialize the clock
    initializeClock();

    // Log the initialization message
    logMessage("OSS started");

    // Initialize PCBs
    for (int i = 0; i < MAX_CHILD_PROCESSES; i++) {
        processTable[i].occupied = 0;
        // Initialize other PCB fields...
    }

    // Rest of your oss code...

    // Detach and remove shared memory (don't forget to do this when your program exits)
    shmdt(systemClock);
    shmctl(shmid, IPC_RMID, NULL);

    // Close the log file
    fclose(logFile);

    return 0;
}
