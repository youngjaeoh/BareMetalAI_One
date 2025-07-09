#ifndef INC_QUEUE_H_
#define INC_QUEUE_H_

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_QUEUE_SIZE 1000

// Embedded System에서 maxSize를 정적으로 정의해야 하나?
typedef struct {
    uint8_t buffer[MAX_QUEUE_SIZE];
    int head;
    int tail;
} CircularQueue;

void queue_init(CircularQueue *q);
bool queue_enqueue(CircularQueue *q, uint8_t data);
uint8_t queue_dequeue(CircularQueue *q);
bool queue_is_full(CircularQueue *q);
bool queue_is_empty(CircularQueue *q);
uint8_t queue_peek(CircularQueue *q);
uint8_t queue_size(CircularQueue *q) ;

#ifdef __cplusplus
}
#endif

#endif /* INC_QUEUE_H_ */
