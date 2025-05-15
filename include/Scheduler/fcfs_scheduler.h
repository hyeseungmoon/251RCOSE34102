//
// Created by hyese on 2025-05-16.
//

#ifndef FCFS_SCHEDULER_H
#define FCFS_SCHEDULER_H

#include <scheduler_interface.h>

typedef struct FCFSScheduler {
    SchedulerInterface* interface;


}FCFSScheduler;

FCFSScheduler* fcfs_scheduler_constructor(Cpu* cpu);
void fcfs_insert_new_process(struct FCFSScheduler* self, ProcessControlBlock*, int arrive_time);
void fcfs_exit(struct FCFSScheduler* self);
void fcfs_io_interrupt(struct FCFSScheduler* self);
void fcfs_interrupt(struct FCFSScheduler* self);
void fcfs_dispatch(struct FCFSScheduler* self);

#endif //FCFS_SCHEDULER_H
