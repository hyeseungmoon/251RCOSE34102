//
// Created by hyese on 2025-04-26.
//

#ifndef READY_QUEUE_H
#define READY_QUEUE_H

#include <../Component/process.h>
#include <queue_interface.h>
#include <../Component/cpu.h>

typedef struct SchedulerInterface {
    IQueue* ready_queue;
    IQueue* waiting_queue;
    Cpu* core;
    ProcessControlBlock* current_pcb;

    void (*insert_new_process)(struct SchedulerInterface* self, ProcessControlBlock*, int arrive_time);
    void (*exit)(struct SchedulerInterface* self);
    void (*io_interrupt)(struct SchedulerInterface* self);
    void (*interrupt)(struct SchedulerInterface* self);
    void (*dispatch)(struct SchedulerInterface* self);
}SchedulerInterface;

#endif //READY_QUEUE_H
