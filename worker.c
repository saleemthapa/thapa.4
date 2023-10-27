/*#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_CHILD_PROCESSES 18

// Define the PCB (Process Control Block) structure
struct PCB {
    int occupied; // either true or false
    pid_t pid; // process id of this child
    int startSeconds; // time when it was created
    int startNano; // time when it was created
    int serviceTimeSeconds; // total seconds it has been "scheduled"
    int serviceTimeNano; // total nanoseconds it has been "scheduled"
    int eventWaitSec; // when does its event happen?
    int eventWaitNano; // when does its event happen?
    int blocked; // is this process waiting on an event?
};

struct PCB processTable[MAX_CHILD_PROCESSES];

// Function to initialize the PCBs
void initializePCBs() {
    int i;
    for (i = 0; i < MAX_CHILD_PROCESSES; i++) {
        processTable[i].occupied = 0;
        // Initialize other PCB fields...
    }
 printf("PCB intializing");
}

int main(int argc, char* argv[]) {
    // Rest of your oss code...

    // Initialize PCBs
    initializePCBs();

    // Rest of your oss code...

    return 0;
}

*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <time.h>
#include "mytime.h"
// Initialize the random number generator with the current time
//srand(time(NULL));

// Define the range for time usage (e.g., 1000 to 5000 nanoseconds)
int minTime = 1000;
int maxTime = 5000;
// Define a threshold for termination probability (e.g., 10% chance of termination)
int terminationThreshold = 10;

/*// Generate a random number between 1 and 100 (inclusive)
int randomValue = 1 + rand() % 100;
*/
// Define a structure for the message sent from oss to worker
struct Message {
    long mtype;
    int timeSlice; // The time slice for the worker to run
};

int main(int argc, char* argv[]) {
    // Initialize any necessary data structures

    // Attach to the message queue created by oss
    key_t key = 1994 /* Specify the same message queue key as in oss.c */;
    int msgid = msgget(key, 0666);

    if (msgid == -1) {
        perror("worker: Failed to attach to the message queue");
        exit(EXIT_FAILURE);
    }
srand(getCustomTime());
    while (1) {
        // Wait for a message from oss
        struct Message message;
        if (msgrcv(msgid, &message, sizeof(message.timeSlice), getpid(), 0) == -1) {
            perror("worker: Failed to receive message from oss");
            exit(EXIT_FAILURE);
        }

        // Simulate work based on the received time
	int usedTime = minTime + (rand() % (maxTime - minTime + 1));
        // Send a message to oss indicating the time used
        struct Message response;
        response.mtype = 1; // Use a different message type
        response.timeSlice = usedTime;

        if (msgsnd(msgid, &response, sizeof(response.timeSlice), 0) == -1) {
            perror("worker: Failed to send message to oss");
            exit(EXIT_FAILURE);
        }
	// Generate a random number between 1 and 100 (inclusive)
	int randomValue = 1 + rand() % 100;
        // Check if it's time to terminate
        int terminateProbability = (randomValue <= terminationThreshold) ? 1 : 0;
	if (terminateProbability == 1) {
            // Send a termination message to oss and exit
            struct Message terminationMessage;
            terminationMessage.mtype = 1; // Use a different message type
            terminationMessage.timeSlice = -1; // Use a negative value to indicate termination

            if (msgsnd(msgid, &terminationMessage, sizeof(terminationMessage.timeSlice), 0) == -1) {
                perror("worker: Failed to send termination message to oss");
            }

            exit(0);
        }
    }

    // Cleanup and exit
    return 0;
}

