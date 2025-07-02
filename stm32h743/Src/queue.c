#include "queue.h"
// 필요시 stdint.h, stdbool.h는 queue.h에서 include되어야 함

void queue_init(CircularQueue *q) {
    q->head = 0;
    q->tail = 0;
}

bool queue_is_empty(CircularQueue *q) {
    return (q->head == q->tail);
}

bool queue_is_full(CircularQueue *q) {
    return ((q->tail + 1) % MAX_QUEUE_SIZE == q->head);
}

bool queue_enqueue(CircularQueue *q, uint8_t data) {
    if (queue_is_full(q)) {
        return false;
    }
    q->buffer[q->tail] = data;
    q->tail = (q->tail + 1) % MAX_QUEUE_SIZE;
    return true;
}

uint8_t queue_dequeue(CircularQueue *q) {
    if (queue_is_empty(q)) {
        return -1;  // Error value
    }
    uint8_t data = q->buffer[q->head];
    q->head = (q->head + 1) % MAX_QUEUE_SIZE;
    return data;
}

uint8_t queue_peek(CircularQueue *q) {
    if (queue_is_empty(q)) {
        return -1;  // Error value
    }
    return q->buffer[q->head];
}

uint8_t queue_size(CircularQueue *q) {
    if (q->tail >= q->head) {
        return (q->tail - q->head);
    } else {
        return (MAX_QUEUE_SIZE - q->head + q->tail);
    }
}
