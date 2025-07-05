#ifndef _RADAR_H
#define _RADAR_H
#include <stdint.h>
#include <stdbool.h>
#include "uart.h"
#include "queue.h"

#ifdef __cplusplus
extern "C" {
#endif

// Sleep Breathing Radar Protocol Defines
#define MESSAGE_HEAD 0x55
#define MAX_DATA_LEN 12

// Debug mode - comment out to disable debug output
#define RADAR_DEBUG_MODE
//Sleep radar data header frames
#define SLEEP_INF 0x05          

// There is a global uart buffer typed as queue in main.c
// Need to process the data in the queue
void radar_data_process(CircularQueue *queue);
void makechecksum(uint8_t* data, uint16_t length);
void send_to_radar(uint8_t* data, uint16_t length);

#ifdef __cplusplus
}
#endif

#endif
