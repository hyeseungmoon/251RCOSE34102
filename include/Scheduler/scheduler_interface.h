//
// Created by hyese on 2025-04-26.
//

#ifndef READY_QUEUE_H
#define READY_QUEUE_H

#include <Component/process.h>
#include <queue_interface.h>
#include <Component/cpu.h>


typedef struct ChartObject{
    int pid;
    int start_time;
    int end_time;
}ChartObject;

typedef struct IScheduler {
    IQueue* ready_queue;
    IQueue* waiting_queue;

    int pid_timestamp[2000];
    int timestamp_cnt;

    Cpu* core;
    ProcessControlBlock* current_pcb;
    int time_quantum;
    int total_waiting_time;
    int total_turnaround_time;
    int total_process_count;

    void (*add_waiting_queue)(struct IScheduler* self, ProcessControlBlock*, const int arrive_time);
    void (*add_ready_queue)(const struct IScheduler* self, ProcessControlBlock*);
    void (*exit_process)(struct IScheduler* self);
    void (*dispatch)(struct IScheduler* self);
    void (*run_timestep)(struct IScheduler* self);
    void (*start)(struct IScheduler* self);
}IScheduler;

IScheduler* base_scheduler_constructor(IQueue* ready_queue, const int time_slice);
void add_waiting_queue(IScheduler* self, ProcessControlBlock* pcb, const int target_time);
void add_ready_queue(const IScheduler* self, ProcessControlBlock*);
void exit_process(IScheduler* self);
void dispatch(IScheduler* self);
void run_timestep(IScheduler* self);
void start(IScheduler* self);

#endif //READY_QUEUE_H
