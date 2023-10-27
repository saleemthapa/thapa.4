#include <stdio.h>
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
   /* int i;
    for (i = 0; i < MAX_CHILD_PROCESSES; i++) {
        processTable[i].occupied = 0;
        // Initialize other PCB fields...
    }*/
 printf("PCB intializing");
}

int main(int argc, char* argv[]) {
    // Rest of your oss code...

    // Initialize PCBs
    initializePCBs();

    // Rest of your oss code...

    return 0;
}

