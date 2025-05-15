//
// Created by hyese on 2025-04-26.
//

#ifndef PROCESS_H
#define PROCESS_H

#include <operation.h>
#include <time.h>

typedef enum ProcessState {
    CREATE,
    READY,
    RUNNING,
    WAITING,
    TERMINATED
}ProcessState;

typedef struct Process {
    int data;
    Operation* text; // list of operation
}Process;

typedef struct ProcessControlBlock {
    unsigned int pid;
    unsigned int program_counter;
    ProcessState process_state;
    Process *process;
    int register_backup;

    int turnaround_time;
    int waiting_time;
    int arrive_time;
}ProcessControlBlock;

#endif //PROCESS_H
