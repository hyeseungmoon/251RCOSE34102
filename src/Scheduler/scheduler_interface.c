//
// Created by hyese on 2025-05-21.
//

#include <Scheduler/scheduler_interface.h>
#include <DataStructure/fifo_queue.h>
#include <DataStructure/priority_queue.h>
#include <Component/process.h>
#include <Component/cpu.h>
#include <stdio.h>

#define DEBUG
//#define TIMESLICE

#define WAITING_QUEUE_CAPACITY 20

extern unsigned int cur_time;
unsigned int current_execution_time = 0;

typedef struct Job {
    unsigned int target_time;
    ProcessControlBlock *pcb;
}Job;

int job_cmp(const void *_o1, const void *_o2) {
    Job* o1 = (Job*)_o1;
    Job* o2 = (Job*)_o2;
    if (o1->target_time == o2->target_time)
        return o1->pcb->pid > o2->pcb->pid;
    return o1->target_time > o2->target_time;
}

IScheduler* base_scheduler_constructor(IQueue* ready_queue, const int time_slice) {
    IScheduler* scheduler = malloc(sizeof(IScheduler));
    scheduler->ready_queue = ready_queue;
    scheduler->waiting_queue = priority_queue_constructor(WAITING_QUEUE_CAPACITY, job_cmp);
    scheduler->core = cpu_constructor();
    scheduler->current_pcb = NULL;
    scheduler->time_slice = time_slice;

    scheduler->add_waiting_queue = add_waiting_queue;
    scheduler->add_ready_queue = add_ready_queue;
    scheduler->exit_process = exit_process;
    scheduler->dispatch = dispatch;
    scheduler->run_timestep = run_timestep;
    scheduler->start = start;
    return scheduler;
}

void add_waiting_queue(const IScheduler* self, ProcessControlBlock* pcb, const int target_time) {
    IQueue* job_queue = self->waiting_queue;
    Job* job = malloc(sizeof(Job));
    job->target_time = target_time;
    job->pcb = pcb;
    pcb->program_counter = self->core->program_counter;
    if(pcb->arrive_time == -1)
        pcb->arrive_time = target_time;
    job_queue->enqueue(job_queue, job);
}

ProcessControlBlock* check_queue(const IQueue* queue) {
    if (queue->is_empty(queue)) {
        // queue 비어있음 -> 무조건 NULL 반환
        return NULL;
    }
    Job *next_job = queue->peek(queue);
    if (cur_time == next_job->target_time) {
        // 다음 job이 enqueue 돼야하는 시점
        queue->dequeue(queue);
        ProcessControlBlock* next_pcb = next_job->pcb;
        free(next_job);
#ifdef DEBUG
        printf("%d ADD READY pid %d\n", cur_time, next_pcb->pid);
#endif
        return next_pcb;
    }
    return NULL;
}

void add_ready_queue(const IScheduler* self, ProcessControlBlock* pcb) {
    IQueue* ready_queue = self->ready_queue;
    pcb->last_ready_queue_inserted_time = cur_time;
    pcb->process_state = READY;
    ready_queue->enqueue(ready_queue, pcb);
}

void exit_process(IScheduler* self) {
    ProcessControlBlock* pcb = self->current_pcb;
    self->core->state = IDLE;
    pcb->process_state = TERMINATED;
    pcb->turnaround_time = cur_time - pcb->arrive_time;
    self->current_pcb = NULL;

#ifdef DEBUG
    printf("%d FINISHING PROCESS PID %d TT : %d WT : %d\n", cur_time, pcb->pid, pcb->turnaround_time, pcb->waiting_time);
#endif
}

void dispatch(IScheduler* self) {
    // 현재 pcb와 ready_queue의 다음 pcb 교환
    current_execution_time = 0;
    IQueue* ready_queue = self->ready_queue;
    ProcessControlBlock* pcb = self->current_pcb;
    Cpu* core = self->core;

    if (pcb) {
        pcb->program_counter = core->program_counter;
        pcb->process_state = READY;
        add_ready_queue(self, pcb);
    }

    if (ready_queue->is_empty(ready_queue)) {
        // queue가 비어있는 경우 ERROR
        return;
    }

    ProcessControlBlock* next_pcb = ready_queue->dequeue(ready_queue);
    next_pcb->waiting_time += cur_time - next_pcb->last_ready_queue_inserted_time;
    next_pcb->process_state = RUNNING;
    core->program_counter = next_pcb->program_counter;
    core->current_process = next_pcb->process;
    self->current_pcb = next_pcb;
#ifdef DEBUG
    printf("%d Dispatching from process pid_%d -> pid_%d\n", cur_time, pcb ? pcb->pid : -1, next_pcb->pid);
#endif
    return;
}

void execute_cpu(IScheduler* self) {
    Cpu* core = self->core;
    // CPU 연산 수행
    core->execute_operation(core);
    if (core->state == COMPUTATION) {
        cur_time++;
        current_execution_time++;
        self->current_pcb->executed_time++;
    }
    else if (core->state == IO) {
        self->add_waiting_queue(self, self->current_pcb, core->end_time);
        self->current_pcb = NULL;
        self->core->state = IDLE;
    }
    else if (core->state == RETURN) {
        self->exit_process(self);
    }
}

void run_timestep(IScheduler* self) {
    IQueue* ready_queue = self->ready_queue;
    ProcessControlBlock* current_pcb = self->current_pcb;
    Cpu* core = self->core;

    // 현재 타임 스텝에 추가 해야 하는 작업이 waiting queue에 있는지 탐색 후 존재할 시 추가
    ProcessControlBlock* next_pcb = check_queue(self->waiting_queue);
    while (next_pcb) {
        self->add_ready_queue(self, next_pcb);
        next_pcb = check_queue(self->waiting_queue);
    }

    // 현재 pcb는 비어 있지만, Ready 큐에 있을 시 pcb 가져오고 current_pcb 갱신
    if (current_pcb == NULL && !ready_queue->is_empty(ready_queue)) {
        self->dispatch(self);
        current_pcb = self->current_pcb;
    }

    // Ready와 current_pcb도 없을 경우 해당 timeslice 는 아무것도 하지 않고 IDLE로 보내기
    if (current_pcb == NULL && ready_queue->is_empty(ready_queue)) {
        cur_time++;
        current_execution_time++;
        return;
    }

    // timeslice expiration 처리
    if (current_execution_time == self->time_slice) {
        if (core->fetch_next_operation(core).type == RET) {
            core->execute_operation(core);
            exit_process(self);
        }
#ifdef TIMESLICE
         printf("%d TIMESLICE EXPIRATION! \n", cur_time);
#endif
        self->dispatch(self);
        return;
    }

    // CPU  연산 수행
    execute_cpu(self);
}

void start(IScheduler* self) {
    cur_time = 0;
    current_execution_time = 0;

    IQueue* ready_queue = self->ready_queue;
    IQueue* job_queue = self->waiting_queue;
    while (!ready_queue->is_empty(ready_queue) || !job_queue->is_empty(job_queue) || self->current_pcb) {
        self->run_timestep(self);
    }
}