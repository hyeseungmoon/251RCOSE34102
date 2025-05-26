#include <queue_interface.h>
#include <priority_queue.h>
#include <stdlib.h>

void priority_queue_enqueue(void *_self, void *item) {
    PriorityQueue* self = _self;
    if (!self->interface.is_full(self)) {
        self->size++;
        int cur = self->size;
        self->items[cur] = item;
        while (cur > 1) {
            const int parent = cur / 2;
            if (self->cmp(self->items[parent], self->items[cur])) {
                void *tmp = self->items[parent];
                self->items[parent] = self->items[cur];
                self->items[cur] = tmp;
                cur = parent;
            }
            else break;
        }
    }
}

bool priority_queue_is_full(void *_self) {
    PriorityQueue* self = _self;
    if (self->size == self->capacity) return true;
    return false;
}

void* priority_queue_dequeue(void *_self) {
    PriorityQueue* self = _self;
    void* ret = self->items[1];
    self->items[1] = self->items[self->size];
    self->size--;

    if (!self->interface.is_empty(self)) {
        int cur = 1;
        while (cur * 2 <= self->size) {
            int idx = cur * 2;
            if (idx + 1 <= self->size && self->cmp(self->items[idx], self->items[idx + 1])) idx = idx + 1;
            if (self->cmp(self->items[cur], self->items[idx])) {
                void *tmp = self->items[cur];
                self->items[cur] = self->items[idx];
                self->items[idx] = tmp;
                cur = idx;
            }
            else break;
        }
    }
    return ret;
}

bool priority_queue_is_empty(void *_self) {
    PriorityQueue* self = _self;
    if (self->size == 0) return true;
    return false;
}

void* priority_queue_peek(void *_self) {
    PriorityQueue* self = _self;
    if (self->interface.is_empty(self)) {
        return NULL;
    }
    void *ret = self->items[1];
    return ret;
}

void priority_queue_rebalance(void *_self) {
    PriorityQueue* self = _self;

    // 빈 큐이거나 원소가 1개면 rebalancing 불필요
    if (self->size <= 1) return;

    // 모든 non-leaf 노드에 대해 heapify 수행 (bottom-up)
    for (int i = self->size / 2; i >= 1; i--) {
        int cur = i;

        // 현재 노드를 루트로 하는 서브트리를 힙으로 만들기
        while (cur * 2 <= self->size) {
            int left_child = cur * 2;
            int right_child = left_child + 1;
            int target = left_child;

            // 오른쪽 자식이 존재하고 더 우선순위가 높으면 선택
            if (right_child <= self->size &&
                self->cmp(self->items[left_child], self->items[right_child])) {
                target = right_child;
            }

            // 부모가 자식보다 우선순위가 낮으면 교환
            if (self->cmp(self->items[cur], self->items[target])) {
                void *tmp = self->items[cur];
                self->items[cur] = self->items[target];
                self->items[target] = tmp;
                cur = target;
            } else {
                break; // 힙 속성이 만족되면 중단
            }
        }
    }
}

IQueue* priority_queue_constructor(int capacity, int (*cmp)(const void*, const void*)) {
    PriorityQueue* q = malloc(sizeof(PriorityQueue));
    q->items = malloc(sizeof(void*)*capacity + 1);
    q->size = 0;
    q->capacity = capacity;
    q->cmp = cmp;
    q->priority_queue_rebalance = priority_queue_rebalance;

    q->interface.enqueue = priority_queue_enqueue;
    q->interface.dequeue = priority_queue_dequeue;
    q->interface.is_full = priority_queue_is_full;
    q->interface.is_empty = priority_queue_is_empty;
    q->interface.peek = priority_queue_peek;
    return (IQueue*)q;
}