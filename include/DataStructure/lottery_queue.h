//
// Created by hyese on 2025-05-26.
//

#ifndef LOTTERY_QUEUE_H
#define LOTTERY_QUEUE_H

typedef struct Node {
    int lottery;
    void* item;
    struct Node* next;
}Node;

typedef struct LotteryQueue {
    IQueue interface;
    Node* head;
    int size;
}LotteryQueue;

void lottery_queue_enqueue(void *self, void *item);
void* lottery_queue_dequeue(void *self);
bool lottery_queue_is_empty(void *self);
bool lottery_queue_is_full(void *self);
void* lottery_queue_peek(void *self);
IQueue* lottery_queue_constructor();

#endif //LOTTERY_QUEUE_H
