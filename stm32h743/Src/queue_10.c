#include "queue_10.h"
// 필요시 stdint.h, stdbool.h는 queue.h에서 include되어야 함

/* ===================================================================
 * 큐(Queue)를 관리하기 위한 헬퍼 함수들
 * ===================================================================*/
void ProbQueue_Init(ProbQueue* queue) {
    queue->head = 0;
    queue->tail = 0;
    queue->size = 0;
    memset(queue->buffer, 0, sizeof(queue->buffer));
}

int ProbQueue_IsFull(const ProbQueue* queue) {
    return queue->size == MAX_PROB_QUEUE_SIZE;
}

int ProbQueue_Size(const ProbQueue* queue) {
    return queue->size;
}

void ProbQueue_Enqueue(ProbQueue* queue, const PredictionProbs* data) {
    if (ProbQueue_IsFull(queue)) {
        // 큐가 가득 찼다면, 가장 오래된 데이터를 덮어쓰고 head를 이동
        queue->buffer[queue->tail] = *data;
        queue->head = (queue->head + 1) % MAX_PROB_QUEUE_SIZE;
        queue->tail = (queue->tail + 1) % MAX_PROB_QUEUE_SIZE;
    } else {
        // 큐에 공간이 있다면, 데이터를 추가하고 size 증가
        queue->buffer[queue->tail] = *data;
        queue->tail = (queue->tail + 1) % MAX_PROB_QUEUE_SIZE;
        queue->size++;
    }
}

// 큐의 논리적 인덱스(0: 가장 오래된 데이터, size-1: 최신 데이터)로 데이터에 접근
// 큐에서 데이터를 빼지 않고 확인만 하는 함수
int ProbQueue_Peek(const ProbQueue* queue, int index, PredictionProbs* data) {
    if (index < 0 || index >= queue->size) {
        return 0; // 실패
    }
    int buffer_index = (queue->head + index) % MAX_PROB_QUEUE_SIZE;
    *data = queue->buffer[buffer_index];
    return 1; // 성공
}
