#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <cpu.h>
typedef enum RegisterKey {
    REG1
}RegisterKey;

typedef enum OperationType {
    CPU, //  CPU reg1 10
    MEM, // MEM reg1 100
    RET // RET
}OperationType;

typedef struct Operation {
    OperationType type;
    RegisterKey dest;
    int src;
}Operation;

#endif //INSTRUCTION_H
