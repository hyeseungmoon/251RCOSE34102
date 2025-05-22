//
// Created by hyese on 2025-05-19.
//
#include <process.h>
#include <string.h>
#include <stdlib.h>

#define MAX_OPERATION_LENGTH 100

extern unsigned int cur_time;
extern unsigned int cur_pid;

Operation parse_operation(char *operation_string) {
    Operation ret_operation;

    char *ptr = strtok(operation_string, " ");
    if (!strcmp(ptr, "CPU")) {
        ret_operation.type = CPU;
    }
    else if (!strcmp(ptr, "MEM")) {
        ret_operation.type = MEM;
    }
    else {
        ret_operation.type = RET;
    }
    strtok(NULL, " ");
    ptr = strtok(NULL, " ");
    int src = atoi(ptr);
    ret_operation.src = src;
    return ret_operation;
}

Process* process_constructor(char *s) {
    Process* process = malloc(sizeof(Process));
    process->text = calloc(MAX_OPERATION_LENGTH, sizeof(Operation));
    process->data = 0;

    int j = 0;
    int text_cnt = 0;
    char buff[32];
    for (int i=0;i<strlen(s) + 1;i++) {
        if (s[i] != '\n') buff[j++] = s[i];
        else {
            buff[j] = '\0';
            process->text[text_cnt++] = parse_operation(buff);
            j = 0;
        }
    }
    process->text[text_cnt] = parse_operation(buff);
    process->operation_cnt = text_cnt;
    return process;
}

ProcessControlBlock* process_control_block_constructor(Process* process) {
    ProcessControlBlock* pcb = malloc(sizeof(ProcessControlBlock));
    pcb->pid = cur_pid++;
    pcb->program_counter = 0;
    pcb->process_state = CREATE;
    pcb->process = process;
    pcb->register_backup = 0;

    pcb->turnaround_time = 0;
    pcb->waiting_time = 0;
    pcb->arrive_time = -1;
    pcb->executed_time = 0;
    return pcb;
}
