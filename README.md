# 2025COSE341 운영체제 02분반 Scheduler Simulator

## 특징 #1 OOP in C
Scheduling을 모듈화하여, 재사용하여 다양한 스케줄링 알고리즘 구현 가능  
### 스케줄러 구성요소
* FCFS 스케줄러 : FIFO 큐 + 기본 스케줄링
* SJF 스케줄러 (non-preemptive) : Priority 큐 + 기본 스케줄링
* RR 스케줄러 : FIFO 큐 + timeslice 처리 스케줄링
* Lottery 스케줄러 : Lottery 큐 + 기본 스케줄링
* SJF 스케줄러 (preemptive) : Priority 큐 + Preemptive 스케줄링
* Starvation 스케줄러 : Priority 큐 + starvation 스케줄링

```c++
// Lottery 스케줄러 구현 
IScheduler* lottery_scheduler_constructor() {
    IScheduler* scheduler = priority_non_preemptive_scheduler_constructor(); // priority 기반
    free(scheduler->ready_queue); 
    scheduler->ready_queue = lottery_queue_constructor(); // Ready Queue는 lottery 사용
    return scheduler;
}

void preemptive_add_queue_wrapper(struct IScheduler* self, ProcessControlBlock* pcb) {
    add_ready_queue(self, pcb);
    ... 추가적으로 필요한 내용
}

// Preemptive Priority 스케줄러 구현
IScheduler* priority_preemptive_scheduler_constructor() {
    IScheduler* scheduler = priority_non_preemptive_scheduler_constructor(); // 기본 non-preemptive Base
    scheduler->add_ready_queue = (void (*)(const struct IScheduler *,
                                           ProcessControlBlock *)) preemptive_add_queue_wrapper; // add_ready_queue 메소드만 오버라이드
    return scheduler;
}
```
위 코드처럼 기본적으로 base_scheduler 오브젝트를 기반으로 하여, 각 스케줄러의 특징에 맞게 구성 요소를 갈아 끼우는 것으로 사용 가능하다.  
이는 유연하게 스케줄링 알고리즘을 테스트하고, 도입하기 좋은 기반이 된다.  
### 구현 방식
C++ 가상 함수는 C 언어의 **함수 포인터**를 이용하여 구현  
C++ 추상화 및 다형성은 C언어의 구조체 포인터에서 **동일한 시작주소를 가지면 형변환 가능**하다는 점을 활용하여 구현이 가능하다.

### C언어에서 추상화 구현 예시
```c++
// Queue 인터페이스
typedef struct IQueue {
    void (*enqueue)(void *self, void *item);
    void* (*dequeue)(void *self);
    bool (*is_empty)(void *self);
    bool (*is_full)(void *self);
    void* (*peek)(void *self);
}IQueue;

// Queue 인터페이스를 상속받는 FIFO QUEUE
typedef struct FIFOQueue {
    IQueue interface; // ! 무조건 첫번째 멤버로 선언을 해야함
    void** items;
    int front;
    int rear;
    int size;
    int capacity;
}FIFOQueue;

IQueue* q = fifo_queue_constructor(); // 다형성 사용하여 부모 클래스로 자식 클래스 사용
```

## 특징 #2 유연한 IO 처리
본 프로젝트의 스케줄러는 간단한 CPU 시뮬레이터를 포함하고 있으며, IO와 Dispatch 방식이 실제 OS Kernel에서 처리하는 것과 비슷한 방식을 취한다  
모든 프로그램은 보조 메모리(SSD)에 raw text 형식으로 저장되며 다음과 같은 형식을 취한다.
```text
CPU REG1 10 // CPU 연산을 timeslice 1 만큼 실행
CPU REG1 10
CPU REG1 10
MEM REG1 1 // IO 인터럽트를 발생하고 1 timeslice 만큼 memeory fetch 흉내
CPU REG1 10
MEM REG2 10
RET REG1 10
```
해당 operation 집합은 `MIPS` 명령어 구조의 `operator dest src` 형식을 따왔음.  
CPU와 Scheduler는 언제 IO 인터럽션이 있을지 모르는 상태이며 CPU가 MEM 종류의 명령어를 수행하게 될시, IO 인터럽션을 수행하게 된다.  


## 특징 #3 확장 가능
앞으로 확장 가능한 부분들은 다음과 같다
1. 멀티코어 cpu 지원 및 멀티코어 스케줄링 -> 스케줄러 속 Core변수 배열화
2. 실시간 프로세스 스케줄링
3. CPU 연산 다양화 및, 레지스터 메모리 구현