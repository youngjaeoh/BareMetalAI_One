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

#define SLEEP_INF 0x05          //Sleep radar data header frames
#define BREATH 0x01             //Breathing parameters
#define SCENARIO 0x03           //Scenario evaluation
#define SLEEP_TIME 0x04         //Duration parameters
#define SLEEP_QUALITY 0x05      //Sleep quality

#define BREATH_RATE 0x01        //Breathing rate
#define CHECK_SIGN 0x04         //Detection signal

#define BREATH_HOLD 0x01        //Breath-holding abnormalities
#define BREATH_NULL 0x02        //None
#define BREATH_NORMAL 0x03      //Normal breathing
#define BREATH_MOVE 0x04        //Movement abnormalities
#define BREATH_RAPID 0x05       //Acute respiratory abnormalities

#define CLOSE_AWAY_BED 0x07     //Getting in and out of bed judgment
#define SLEEP_STATE 0x08        //Sleep state judgment

#define AWAY_BED 0x00           //Leaving the bed
#define CLOSE_BED 0x01          //Get into bed

#define AWAKE 0x00              //sleep state:Awake
#define LIGHT_SLEEP 0x01        //sleep state:Light sleep
#define DEEP_SLEEP 0x02         //sleep state:deep sleep
#define SLEEP_NULL 0x03         //no sleep state

#define AWAKE_TIME 0x01         //Awake time
#define LIGHT_SLEEP_TIME 0x02   //Light sleep time
#define DEEP_SLEEP_TIME 0x03    //Deep sleep time

#define SLEEP_SCORE 0x01        //Sleep quality score

// There is a global uart buffer typed as queue in main.c
// Need to process the data in the queue
void radar_data_process(CircularQueue *queue);
void SleepInfer(uint8_t* inf);
void SleepTimeCalculate(uint8_t inf1, uint8_t inf2, uint8_t inf3, uint8_t inf4);
void ShowData(uint8_t inf[]);
uint16_t CalculateCrc16(unsigned char *lpuc_Frame, unsigned short int lus_Len);
void makechecksum(uint8_t* data, uint8_t length);
void send_to_radar(uint8_t* data, uint8_t length);

#ifdef __cplusplus
}
#endif

#endif
