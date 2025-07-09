#ifndef INC_QUEUE_10_H_
#define INC_QUEUE_10_H_

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_PROB_QUEUE_SIZE 10
#define NUM_CLASSES 3

// 3개의 확률 값을 저장하는 구조체
typedef struct {
    float probs[NUM_CLASSES];
} PredictionProbs;

// PredictionProbs 구조체를 10개 저장하는 큐
typedef struct {
    PredictionProbs buffer[MAX_PROB_QUEUE_SIZE];
    int head;
    int tail;
    int size;
} ProbQueue;

void ProbQueue_Init(ProbQueue* queue);
void ProbQueue_Enqueue(ProbQueue* queue, const PredictionProbs* data);
int ProbQueue_IsFull(const ProbQueue* queue);
int ProbQueue_Peek(const ProbQueue* queue, int index, PredictionProbs* data);
int ProbQueue_Size(const ProbQueue* queue);

#ifdef __cplusplus
}
#endif

#endif /* INC_QUEUE_H_ */
