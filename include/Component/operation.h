#ifndef INSTRUCTION_H
#define INSTRUCTION_H

typedef enum OperationType {
    CPU, //  CPU reg1 10
    MEM, // MEM reg1 100
    RET // RET
}OperationType;

typedef struct Operation {
    OperationType type;
    int src;
}Operation;

#endif //INSTRUCTION_H
