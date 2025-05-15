//
// Created by hyese on 2025-05-16.
//

#include <stdlib.h>
#include <fcfs_scheduler.h>
#include <fifo_queue.h>

#define MAX_READY_COUNT 10
#define MAX_WAITING_COUNT 10

FCFSScheduler* fcfs_scheduler_constructor(Cpu* cpu) {
    FCFSScheduler* self = malloc(sizeof(FCFSScheduler));
    self->interface->ready_queue = fifo_queue_constructor(MAX_READY_COUNT);
    self->interface->waiting_queue = fifo_queue_constructor(MAX_WAITING_COUNT);
    self->interface->core = cpu;
    self->interface->current_pcb = NULL;

    // TODO : connect function override
    return self;
}

void fcfs_insert_new_process(FCFSScheduler* self, ProcessControlBlock* pcb, int arrive_time) {
    IQueue* ready_queue = self->interface->ready_queue;
    pcb->arrive_time = arrive_time;
    self->interface->ready_queue->enqueue(ready_queue, pcb);
}

void fcfs_exit(FCFSScheduler* self) {
    self->interface->current_pcb->process_state = TERMINATED;
    self->interface->current_pcb = NULL;
}

void fcfs_interrupt(FCFSScheduler *self) {
    IQueue* ready_queue = self->interface->ready_queue;

    self->interface->dispatch(self);

    self->interface->current_pcb->process_state = READY;
    self->interface->ready_queue->enqueue(ready_queue, self->interface->current_pcb);
}

void cleanup_current_process(IQueue* ready_queue, const Cpu* cpu, ProcessControlBlock *pcb) {
    pcb->register_backup = cpu->reg;
    pcb->program_counter = cpu->program_counter;
    pcb->process_state = READY;
    ready_queue->enqueue(ready_queue, pcb);
}

ProcessControlBlock* init_next_pcb(IQueue* ready_queue, Cpu* cpu) {
    ProcessControlBlock* next_pcb = ready_queue->dequeue(ready_queue);
    cpu->reg =  next_pcb->register_backup;
    cpu->program_counter = next_pcb->program_counter;
    next_pcb->process_state = RUNNING;
    return next_pcb;
}

void fcfs_dispatch(FCFSScheduler* self) {
    IQueue* ready_queue = self->interface->ready_queue;
    ProcessControlBlock *current_pcb = self->interface->current_pcb;
    cleanup_current_process(ready_queue, self->interface->core, current_pcb);
    ProcessControlBlock *next_pcb = init_next_pcb(ready_queue, self->interface->core);
    self->interface->current_pcb = next_pcb;
}