#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include <stdbool.h>
#include <queue_interface.h>

typedef struct PriorityQueue {
    IQueue interface;
    void** items;
    int size;
    int capacity;

    int (*cmp)(const void*, const void*);
    void (*priority_queue_rebalance)(void *_self);
}PriorityQueue;

void priority_queue_enqueue(void *self, void *item);
void* priority_queue_dequeue(void *self);
bool priority_queue_is_empty(void *self);
bool priority_queue_is_full(void *self);
void* priority_queue_peek(void *self);
void priority_queue_rebalance(void *_self);
IQueue* priority_queue_constructor(int capacity, int (*cmp)(const void*, const void*));

#endif //PRIORITY_QUEUE_H
