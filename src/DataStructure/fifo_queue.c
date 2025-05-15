#include <fifo_queue.h>
#include <stdlib.h>

void fifo_queue_enqueue(void *_self, void *item) {
    FIFOQueue *self = _self;
    if (!self->interface.is_full(self)) {
        self->items[self->rear] = item;
        self->rear = (self->rear + 1) % self->capacity;
        self->size++;
    }
}

void* fifo_queue_dequeue(void *_self) {
    FIFOQueue *self = _self;
    if (!self->interface.is_empty(self)) {
        void *ret = self->items[self->front];
        self->front = (self->front + 1) % self->capacity;
        self->size--;
        return ret;
    }
    return NULL;
}

bool fifo_queue_is_empty(void *_self) {
    FIFOQueue *self = _self;
    return self->size == 0;
}

bool fifo_queue_is_full(void *_self) {
    FIFOQueue *self = _self;
    return self->size == self->capacity;
}

void* fifo_queue_peek(void *_self) {
    FIFOQueue *self = _self;
    if (!self->interface.is_empty(self)) {
        return self->items[self->front];
    }
    return NULL;
}

int array_fifo_queue_delete(void *_self) {
    FIFOQueue *self = _self;
    int deleted_item_cnt = 0;
    while (!self->interface.is_empty(self)) {
        self->interface.dequeue(self);
        deleted_item_cnt++;
    }
    free(self->items);
    free(self);
    return deleted_item_cnt;
}

IQueue* fifo_queue_constructor(int capacity) {
    FIFOQueue* q = malloc(sizeof(FIFOQueue));
    q->front = 0;
    q->rear = 0;
    q->size = 0;
    q->capacity=capacity;
    q->interface.enqueue = fifo_queue_enqueue;
    q->interface.dequeue = fifo_queue_dequeue;
    q->interface.is_empty = fifo_queue_is_empty;
    q->interface.is_full = fifo_queue_is_full;
    q->interface.peek = fifo_queue_peek;
    q->items = malloc(sizeof(void*)*capacity);
    return (IQueue*)q;
}
