#ifndef COSE341_CPU_H
#define COSE341_CPU_H

#include <process.h>
#include <operation.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define REG_CNT 1
#define DATA_LOAD_TIME 3
#define COMPUTATION_TIME 1

typedef int Register;

typedef enum CpuState {
    COMPUTATION,
    IO,
    IDLE
}CpuState;

typedef struct Cpu{
    Register reg;
    int program_counter;
    CpuState state;
    Process *current_process;

    Operation (*fetch_next_operation)(struct Cpu* self);
    int (*execute_operation)(struct Cpu* self, Operation* operation);
}Cpu;

#endif //COSE341_CPU_H