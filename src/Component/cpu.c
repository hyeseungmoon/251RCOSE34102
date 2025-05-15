#include <cpu.h>
#include <unistd.h>



Operation* fetch_next_operation(Cpu* self) {
    self->program_counter++;
    return &self->current_process->text[self->program_counter];
}

int execute_operation(Cpu* self, const Operation* operation) {
    sleep(COMPUTATION_TIME);
    switch (operation->type) {
        case CPU:
            self->state = COMPUTATION;
            self->reg = operation->src;
        case MEM:
            self->state = MEM;
            break;
        case RET:
            self->current_process = NULL;
            self->state = IDLE;
            break;
    }
    return COMPUTATION_TIME;
}