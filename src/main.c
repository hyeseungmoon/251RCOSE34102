#include <process.h>
#include <stdio.h>
#include <Scheduler/scheduler.h>

unsigned int cur_time = 0;

int get_cpu_burst_time(const Process* process) {
    int burst_time = 0;
    for(int i=0;i<process->operation_cnt;i++){
        Operation o = process->text[i];
        if(o.type == CPU) {
            burst_time++;
        }
    }
    return burst_time;
}

char* read_program(const char* file_name) {
    FILE *file;
    char *buffer;
    long file_size;

    // 파일 열기
    file = fopen(file_name, "r");
    if (file == NULL) {
        perror("파일을 열 수 없습니다");
        return NULL;
    }

    // 파일 크기 구하기
    fseek(file, 0, SEEK_END);
    file_size = ftell(file);
    rewind(file);

    // 버퍼 메모리 할당
    buffer = (char *)malloc(sizeof(char) * (file_size + 1));
    if (buffer == NULL) {
        perror("메모리 할당 실패");
        fclose(file);
        return NULL;
    }

    // 파일 읽기
    size_t read_size = fread(buffer, 1, file_size, file);
    buffer[read_size] = '\0'; // 문자열 종료 문자 추가

    return buffer;
}

PCBWithPriority* priority_pcb_constructor(ProcessControlBlock* pcb, int priority) {
    PCBWithPriority* o = malloc(sizeof(PCBWithPriority));
    o->pcb = *pcb;
    o->priority = priority;

    return o;
}

int main(void) {
    int time_quantum;
    printf("input time_quantum : ");
    scanf("%d", &time_quantum);

    int program_count = 0;
    printf("input program count : ");
    scanf("%d", &program_count);
    char file_names[256][128];
    int arrive_time[256];
    int priority[256];


    for (int i=0;i<program_count;i++) {
        char name[64];
        printf("input arrive_time priority program_name eg. 0 p1\n");
        scanf("%d %d %s", &arrive_time[i], &priority[i], file_names[i]);
    }

    IScheduler* fcfs_scheduler = fcfs_scheduler_constructor();
    IScheduler* non_preemptive_sjf = sjf_non_preemptive_scheduler_constructor();
    IScheduler* preemptive_sjf = sjf_preemptive_scheduler_constructor();
    IScheduler* non_preemptive_priority = priority_non_preemptive_scheduler_constructor();
    IScheduler* preemptive_priority = priority_preemptive_scheduler_constructor();
    IScheduler* round_robin = round_robin_scheduler_constructor(time_quantum);
    IScheduler* starvation = starvation_priority_preemptive_scheduler_constructor();

    for (int i=0;i<program_count;i++) {
        char* buffer = read_program(file_names[i]);
        Process *p = process_constructor(buffer);

        ProcessControlBlock *pcb_fcfs = process_control_block_constructor(p, i);
        ProcessControlBlock *_pcb_p_sjf = process_control_block_constructor(p, i);
        ProcessControlBlock *_pcb_sjf = process_control_block_constructor(p, i);
        ProcessControlBlock *_pcb_non_p_priority = process_control_block_constructor(p, i);
        ProcessControlBlock *_pcb_p_priority = process_control_block_constructor(p, i);
        ProcessControlBlock *pcb_rr = process_control_block_constructor(p, i);
        ProcessControlBlock *_pcb_starvation = process_control_block_constructor(p, i);


        PCBWithPriority *pcb_p_sjf = priority_pcb_constructor(_pcb_p_sjf, get_cpu_burst_time(p));
        PCBWithPriority *pcb_sjf = priority_pcb_constructor(_pcb_sjf, get_cpu_burst_time(p));
        PCBWithPriority *pcb_non_p_priority = priority_pcb_constructor(_pcb_non_p_priority, priority[i]);
        PCBWithPriority *pcb_p_priority = priority_pcb_constructor(_pcb_p_priority, priority[i]);
        PCBWithPriority *pcb_starvation = priority_pcb_constructor(_pcb_starvation, get_cpu_burst_time(p));

        fcfs_scheduler->add_waiting_queue(fcfs_scheduler, pcb_fcfs, arrive_time[i]);
        non_preemptive_sjf->add_waiting_queue(non_preemptive_sjf, pcb_sjf, arrive_time[i]);
        preemptive_sjf->add_waiting_queue(preemptive_sjf, pcb_p_sjf, arrive_time[i]);
        non_preemptive_priority->add_waiting_queue(non_preemptive_priority, pcb_non_p_priority, arrive_time[i]);
        preemptive_priority->add_waiting_queue(preemptive_priority, pcb_p_priority, arrive_time[i]);
        round_robin->add_waiting_queue(round_robin, pcb_rr, arrive_time[i]);
        starvation->add_waiting_queue(starvation, pcb_starvation, arrive_time[i]);
        free(buffer);
    }

    printf("** FCFS Scheduler **\n");
    fcfs_scheduler->start(fcfs_scheduler);
    printf("\n\n\n");

    printf("** Non-Preemptive SJF Scheduler **\n");
    non_preemptive_sjf->start(non_preemptive_sjf);
    printf("\n\n\n");

    printf("** Preemptive SJF Scheduler **\n");
    preemptive_sjf->start(preemptive_sjf);
    printf("\n\n\n");

    printf("** Non-Preemptive Priority Scheduler **\n");
    non_preemptive_priority->start(non_preemptive_priority);
    printf("\n\n\n");

    printf("** Preemptive Priority Scheduler **\n");
    preemptive_priority->start(preemptive_priority);
    printf("\n\n\n");

    printf("** Round Robin Scheduler **\n");
    round_robin->start(round_robin);
    printf("\n\n\n");

    printf("** Starvation SJF Scheduler **\n");
    starvation->start(starvation);

    return 0;
}