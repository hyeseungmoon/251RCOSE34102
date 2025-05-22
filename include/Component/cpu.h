#ifndef COSE341_CPU_H
#define COSE341_CPU_H

#include <stdlib.h>

#include "Component/process.h"
#include "Component/operation.h"

#define REG_CNT 1
#define DATA_LOAD_TIME 3

typedef int Register;

typedef enum CpuState {
    COMPUTATION,
    IO,
    IDLE,
    RETURN,
}CpuState;

typedef struct Cpu{
    Register reg;
    int program_counter;
    CpuState state;
    Process *current_process;

    int end_time;
    void (*execute_operation)(struct Cpu* self);
    Operation (*fetch_next_operation)(struct Cpu* self);
}Cpu;

Cpu* cpu_constructor();
Operation fetch_next_operation(Cpu* self);


#endif //COSE341_CPU_H