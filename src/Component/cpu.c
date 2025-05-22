#include <cpu.h>

extern unsigned int cur_time;
extern unsigned int current_execution_time;

Operation fetch_next_operation(Cpu* self) {
    return self->current_process->text[self->program_counter];
}

void execute_operation(Cpu* self) {
    Operation op = fetch_next_operation(self);
    self->program_counter++;
    switch (op.type) {
        case CPU:
            self->state = COMPUTATION;
            break;
        case MEM:
            self->state = MEM;
            self->end_time = cur_time + op.src;
            break;
        case RET:
            self->current_process = NULL;
            self->state = RETURN;
            break;
    }
}

Cpu* cpu_constructor() {
    Cpu* cpu = malloc(sizeof(Cpu));
    cpu->reg = 0;
    cpu->program_counter = 0;
    cpu->end_time = 0;
    cpu->state = IDLE;
    cpu->current_process = NULL;

    cpu->execute_operation = execute_operation;
    cpu->fetch_next_operation = fetch_next_operation;
    return cpu;
}