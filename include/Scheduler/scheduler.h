//
// Created by hyese on 2025-05-22.
//

#ifndef FCFS_SCHEDULER_H
#define FCFS_SCHEDULER_H

#include <Scheduler/scheduler_interface.h>

typedef struct PCBWithPriority {
    ProcessControlBlock pcb;
    int priority;
}PCBWithPriority;

IScheduler* fcfs_scheduler_constructor();
IScheduler* sjf_non_preemptive_scheduler_constructor();
IScheduler* sjf_preemptive_scheduler_constructor();
IScheduler* priority_non_preemptive_scheduler_constructor();
IScheduler* priority_preemptive_scheduler_constructor();
IScheduler* round_robin_scheduler_constructor(int time_quantum);
IScheduler* starvation_priority_preemptive_scheduler_constructor();

#endif //FCFS_SCHEDULER_H
