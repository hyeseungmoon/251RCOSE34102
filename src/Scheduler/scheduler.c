//
// Created by hyese on 2025-05-22.
//

#include <Scheduler/scheduler_interface.h>
#include <Scheduler/scheduler.h>
#include <DataStructure/fifo_queue.h>
#include <DataStructure/lottery_queue.h>
#include <DataStructure/priority_queue.h>

extern unsigned int cur_time;

int pcb_priority_cmp(const void *_o1, const void *_o2) {
    PCBWithPriority* o1 = (PCBWithPriority*)_o1;
    PCBWithPriority* o2 = (PCBWithPriority*)_o2;
    if (o1->priority == o2->priority)
        return o1->pcb.pid > o2->pcb.pid;
    return o1->priority > o2->priority;
}

IScheduler* fcfs_scheduler_constructor() {
    IScheduler* scheduler = base_scheduler_constructor(fifo_queue_constructor(100), 123456789);
    return scheduler;
}

IScheduler* priority_non_preemptive_scheduler_constructor() {
    IScheduler* scheduler = base_scheduler_constructor(priority_queue_constructor(100, pcb_priority_cmp), 123456789);
    return scheduler;
}

IScheduler* sjf_non_preemptive_scheduler_constructor() {
    return priority_non_preemptive_scheduler_constructor();
}

IScheduler* round_robin_scheduler_constructor(int time_quantum) {
    IScheduler* scheduler = base_scheduler_constructor(fifo_queue_constructor(100), time_quantum);
    return scheduler;
}

void preemptive_add_queue_wrapper(struct IScheduler* self, ProcessControlBlock* pcb) {
    add_ready_queue(self, pcb);
    IQueue* ready_queue = self->ready_queue;
    PCBWithPriority* next_pcb = ready_queue->peek(ready_queue);

    PCBWithPriority* current_pcb = (PCBWithPriority*)self->current_pcb;
    if (current_pcb && current_pcb->priority > next_pcb->priority) {
        Cpu *core = self->core;
        if (core->fetch_next_operation(core).type == RET) {
            core->execute_operation(core);
            exit_process(self);
        }
        self->dispatch(self);
    }
}

IScheduler* priority_preemptive_scheduler_constructor() {
    IScheduler* scheduler = priority_non_preemptive_scheduler_constructor();
    scheduler->add_ready_queue = (void (*)(const struct IScheduler *,
                                           ProcessControlBlock *)) preemptive_add_queue_wrapper;
    return scheduler;
}

void preemptive_sjf_add_queue_wrapper(struct IScheduler* self, ProcessControlBlock* pcb) {
    add_ready_queue(self, pcb);
    IQueue* ready_queue = self->ready_queue;
    PCBWithPriority* next_pcb = ready_queue->peek(ready_queue);

    PCBWithPriority* current_pcb = (PCBWithPriority*)self->current_pcb;
    if (current_pcb && current_pcb->priority > next_pcb->priority) {
        current_pcb->priority -= current_pcb->pcb.executed_time;
        current_pcb->pcb.executed_time = 0;
        Cpu *core = self->core;
        if (core->fetch_next_operation(core).type == RET) {
            core->execute_operation(core);
            exit_process(self);
        }
        self->dispatch(self);
    }
}

IScheduler* sjf_preemptive_scheduler_constructor() {
    IScheduler* scheduler = priority_preemptive_scheduler_constructor();
    scheduler->add_ready_queue = (void (*)(const struct IScheduler *,
                                           ProcessControlBlock *)) preemptive_sjf_add_queue_wrapper;
    return scheduler;
}

void starvation_run_timestep(IScheduler* self) {
    PriorityQueue *priority_queue = (PriorityQueue*)self->ready_queue;
    if(!priority_queue->interface.is_empty(priority_queue) && self->current_pcb) {
        PCBWithPriority *next_pcb = priority_queue->interface.peek(priority_queue);
        PCBWithPriority *current_pcb = self->current_pcb;

        if(current_pcb->priority > next_pcb->priority) {
            self->dispatch(self);
        }
    }

    run_timestep(self);

    for(int i=0;i<priority_queue->size;i++) {
        PCBWithPriority *pcb = (PCBWithPriority*)priority_queue->items[i + 1];
        pcb->priority -= 1;
    }
    priority_queue->priority_queue_rebalance(priority_queue);
}

IScheduler* starvation_priority_preemptive_scheduler_constructor() {
    IScheduler* scheduler = priority_preemptive_scheduler_constructor();
    scheduler->run_timestep =  starvation_run_timestep;
    return scheduler;
}

IScheduler* lottery_scheduler_constructor() {
    IScheduler* scheduler = priority_non_preemptive_scheduler_constructor();
    free(scheduler->ready_queue);
    scheduler->ready_queue = lottery_queue_constructor();
    return scheduler;
}