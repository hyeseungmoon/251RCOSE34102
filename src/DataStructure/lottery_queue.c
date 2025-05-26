//
// Created by hyese on 2025-05-26.
//

#include <queue_interface.h>
#include <lottery_queue.h>
#include <stdlib.h>
#include <time.h>

#include "scheduler.h"

IQueue* lottery_queue_constructor() {
    LotteryQueue* q = malloc(sizeof(LotteryQueue));
    q->size = 0;
    q->head = NULL;  // 초기화 추가
    q->interface.enqueue = lottery_queue_enqueue;
    q->interface.dequeue = lottery_queue_dequeue;
    q->interface.is_empty = lottery_queue_is_empty;
    q->interface.is_full = lottery_queue_is_full;
    q->interface.peek = lottery_queue_peek;

    srand(time(NULL));
    return (IQueue*)q;
}

void lottery_queue_enqueue(void *_self, void *item) {
    LotteryQueue* self = (LotteryQueue*)_self;
    Node* new_node = (Node*)malloc(sizeof(Node));
    PCBWithPriority* i = (PCBWithPriority*)item;
    new_node->lottery = i->priority + 1;
    new_node->item = item;
    new_node->next = NULL;

    if (!self->head) {
        self->head = new_node;
    }
    else {
        Node* n = self->head;
        while (n->next != NULL) n = n->next;
        n->next = new_node;
    }
    self->size++;
}

int get_random(int min, int max) {
    if (max < min) return min;
    return rand() % (max - min + 1) + min;
}

int get_total_lottery(Node* head) {
    Node* n = head;
    int sum = 0;
    while (n != NULL) {
        sum += n->lottery;
        n = n->next;
    }
    return sum;
}

void* lottery_queue_dequeue(void *_self) {
    LotteryQueue* self = (LotteryQueue*)_self;

    if (self->head == NULL) {
        return NULL;  // 빈 큐
    }

    int total_lottery = get_total_lottery(self->head);
    int winner_value = get_random(0, total_lottery);
    Node* prev = NULL;
    Node* current = self->head;
    int current_sum = 0;

    while (current != NULL && current_sum < winner_value) {
        current_sum += current->lottery;
        if (current_sum >= winner_value) {
            break;
        }
        prev = current;
        current = current->next;
    }

    void* item = current->item;

    // 노드 제거
    if (prev == NULL) {
        // 첫 번째 노드 제거
        self->head = current->next;
    } else {
        // 중간 또는 마지막 노드 제거
        prev->next = current->next;
    }

    free(current);
    self->size--;
    return item;
}

bool lottery_queue_is_empty(void *_self) {
    LotteryQueue* self = (LotteryQueue*)_self;
    return self->size == 0;
}

bool lottery_queue_is_full(void *_self) {
    // 동적 할당을 사용하므로 메모리가 부족하지 않은 이상 가득 차지 않음
    return false;
}

void* lottery_queue_peek(void *_self) {
    LotteryQueue* self = (LotteryQueue*)_self;
    if (self->head == NULL) {
        return NULL;
    }

    // lottery queue에서 peek은 다음에 dequeue될 가능성이 높은 항목을 반환
    // 하지만 확률적이므로 정확한 예측은 불가능
    // 여기서는 가장 높은 lottery 값을 가진 항목을 반환
    Node* n = self->head;
    Node* max_node = n;
    int max_lottery = n->lottery;

    while (n != NULL) {
        if (n->lottery > max_lottery) {
            max_lottery = n->lottery;
            max_node = n;
        }
        n = n->next;
    }

    return max_node->item;
}