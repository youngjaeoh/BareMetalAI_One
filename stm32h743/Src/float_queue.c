/**
  ******************************************************************************
  * @file    float_queue.c
  * @brief   Float circular queue implementation for IQ data buffering
  ******************************************************************************
  * @attention
  *
  * This module provides efficient circular queue operations for float data,
  * optimized for real-time IQ signal processing on STM32H743.
  *
  ******************************************************************************
  */

/* Includes ----------------------------------------------------------*/
#include "float_queue.h"
#include <string.h>


/**
 * @brief Initialize a float circular queue
 * @param queue: Pointer to the FloatQueue structure
 * @param buffer: Pointer to the float array buffer
 * @param size: Size of the buffer (number of float elements)
 * @retval None
 */
void float_queue_init(FloatQueue *queue)
{
    if (queue != NULL) {
        // Initialize the queue structure
        memset(queue->buffer, 0, sizeof(queue->buffer)); // Clear the buffer
        queue->head = 0;
        queue->tail = 0;
        queue->count = 0;
    }
}

/**
 * @brief Add a float value to the queue
 * @param queue: Pointer to the FloatQueue structure
 * @param data: Float value to add
 * @retval true if successful, false if queue is full
 */
bool float_queue_enqueue(FloatQueue *queue, float data)
{
    if (queue == NULL || float_queue_is_full(queue)) {
        return false;
    }
    
    queue->buffer[queue->head] = data;
    queue->head = (queue->head + 1) % MAX_FLOAT_QUEUE_SIZE;
    queue->count++;
    
    return true;
}

/**
 * @brief Remove and return a float value from the queue
 * @param queue: Pointer to the FloatQueue structure
 * @param data: Pointer to store the dequeued value
 * @retval true if successful, false if queue is empty
 */
bool float_queue_dequeue(FloatQueue *queue, float *data)
{
    if (data == NULL || float_queue_is_empty(queue)) {
        return false;
    }
    
    *data = queue->buffer[queue->tail];
    queue->tail = (queue->tail + 1) % MAX_FLOAT_QUEUE_SIZE;
    queue->count--;
    
    return true;
}

/**
 * @brief Peek at the next value without removing it
 * @param queue: Pointer to the FloatQueue structure
 * @param data: Pointer to store the peeked value
 * @retval true if successful, false if queue is empty
 */
bool float_queue_peek(FloatQueue *queue, float *data)
{
    if (queue == NULL || data == NULL || float_queue_is_empty(queue)) {
        return false;
    }
    
    *data = queue->buffer[queue->tail];
    return true;
}

/**
 * @brief Check if the queue is empty
 * @param queue: Pointer to the FloatQueue structure
 * @retval true if empty, false otherwise
 */
bool float_queue_is_empty(FloatQueue *queue)
{
    return (queue == NULL) ? true : (queue->count == 0);
}

/**
 * @brief Check if the queue is full
 * @param queue: Pointer to the FloatQueue structure
 * @retval true if full, false otherwise
 */
bool float_queue_is_full(FloatQueue *queue)
{
    return (queue == NULL) ? true : (queue->count >= MAX_FLOAT_QUEUE_SIZE);
}

/**
 * @brief Get the current number of elements in the queue
 * @param queue: Pointer to the FloatQueue structure
 * @retval Number of elements currently in the queue
 */
uint32_t float_queue_count(FloatQueue *queue)
{
    return (queue == NULL) ? 0 : queue->count;
}