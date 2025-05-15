//
// Created by hyese on 2025-04-26.
//

#ifndef QUEUE_INTERFACE_H
#define QUEUE_INTERFACE_H

#include <stdbool.h>

typedef struct IQueue {
    void (*enqueue)(void *self, void *item);
    void* (*dequeue)(void *self);
    bool (*is_empty)(void *self);
    bool (*is_full)(void *self);
    void* (*peek)(void *self);
    struct IQueue* (*constructor)(int capacity);
}IQueue;

#endif //QUEUE_INTERFACE_H
