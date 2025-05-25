//
// Created by hyese on 2025-04-26.
//

#ifndef PROCESS_H
#define PROCESS_H

#include "Component/operation.h"

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
    int operation_cnt;
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
    int last_ready_queue_inserted_time;
    int executed_time;
}ProcessControlBlock;

Process* process_constructor(char *s);

ProcessControlBlock* process_control_block_constructor(Process* process, int pid);

#endif //PROCESS_H
