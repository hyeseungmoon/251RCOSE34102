#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>
#include <queue_interface.h>

typedef struct FIFOQueue {
    IQueue interface;
    void** items;
    int front;
    int rear;
    int size;
    int capacity;
}FIFOQueue;

void fifo_queue_enqueue(void *self, void *item);
void* fifo_queue_dequeue(void *self);
bool fifo_queue_is_empty(void *self);
bool fifo_queue_is_full(void *self);
void* fifo_queue_peek(void *self);
int array_fifo_queue_delete(void *self);
IQueue* fifo_queue_constructor(int capacity);

#endif //QUEUE_H
