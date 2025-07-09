/**
  ******************************************************************************
  * @file    float_queue.h
  * @brief   Float circular queue implementation for IQ data buffering
  ******************************************************************************
  * @attention
  *
  * This module provides a circular queue specifically designed for float
  * data types, optimized for IQ signal processing applications.
  *
  ******************************************************************************
  */

#ifndef __FLOAT_QUEUE_H
#define __FLOAT_QUEUE_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>

#define MAX_FLOAT_QUEUE_SIZE 2000    // Maximum size of the float queue

/* Exported types ------------------------------------------------------------*/
typedef struct {
    float buffer[MAX_FLOAT_QUEUE_SIZE];           // Buffer for float data
    uint32_t head;          // Head index (write position)
    uint32_t tail;          // Tail index (read position)
    uint32_t count;         // Current number of elements in queue
} FloatQueue;

/* Exported constants --------------------------------------------------------*/
#define MAX_FLOAT_QUEUE_SIZE 2000    // Maximum size of the float queue

/* Exported macro ------------------------------------------------------------*/

/* Exported functions prototypes ---------------------------------------------*/

/**
 * @brief Initialize a float circular queue
 * @param queue: Pointer to the FloatQueue structure
 * @param size: Size of the buffer (number of float elements)
 * @retval None
 */
void float_queue_init(FloatQueue *queue);

/**
 * @brief Add a float value to the queue
 * @param queue: Pointer to the FloatQueue structure
 * @param data: Float value to add
 * @retval true if successful, false if queue is full
 */
bool float_queue_enqueue(FloatQueue *queue, float data);

/**
 * @brief Remove and return a float value from the queue
 * @param queue: Pointer to the FloatQueue structure
 * @param data: Pointer to store the dequeued value
 * @retval true if successful, false if queue is empty
 */
bool float_queue_dequeue(FloatQueue *queue, float *data);

/**
 * @brief Peek at the next value without removing it
 * @param queue: Pointer to the FloatQueue structure
 * @param data: Pointer to store the peeked value
 * @retval true if successful, false if queue is empty
 */
bool float_queue_peek(FloatQueue *queue, float *data);

/**
 * @brief Check if the queue is empty
 * @param queue: Pointer to the FloatQueue structure
 * @retval true if empty, false otherwise
 */
bool float_queue_is_empty(FloatQueue *queue);

/**
 * @brief Check if the queue is full
 * @param queue: Pointer to the FloatQueue structure
 * @retval true if full, false otherwise
 */
bool float_queue_is_full(FloatQueue *queue);

/**
 * @brief Get the current number of elements in the queue
 * @param queue: Pointer to the FloatQueue structure
 * @retval Number of elements currently in the queue
 */
uint32_t float_queue_count(FloatQueue *queue);

#ifdef __cplusplus
}
#endif

#endif /* __FLOAT_QUEUE_H */
