#include <math.h>
#include <process.h>
#include <stdio.h>
#include <Scheduler/scheduler.h>

int cur_pid = 1;
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

int main(void) {
    char file_names[][128] = {
        "p1",
        "p2",
        "p3",
        "p4",
        "p5"
    };

    IScheduler* sjf_scheduler = priority_preemptive_scheduler_constructor();
    int priority[] = {4, 3, 2, 1, 4};

    for (int i=0;i<5;i++) {
        char* buffer = read_program(file_names[i]);
        Process *p = process_constructor(buffer);
        ProcessControlBlock *pcb = process_control_block_constructor(p);
        PCBWithPriority *priority_pcb = malloc(sizeof(PCBWithPriority));
        priority_pcb->priority = priority[i];
        priority_pcb->pcb = *pcb;

        sjf_scheduler->add_waiting_queue(sjf_scheduler, (ProcessControlBlock*)priority_pcb, i);
        free(buffer);
    }

    sjf_scheduler->start(sjf_scheduler);

    return 0;
}