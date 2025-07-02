#include "sleepbreathing.h"

// Forward declaration for sprintf function
//int sprintf(char *str, const char *format, ...);

// CRC16 lookup tables - simplified declaration
static const unsigned char cuc_CRCHi[] = {
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40
};

static const unsigned char cuc_CRCLo[] = {
    0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7,
    0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E,
    0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09, 0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9,
    0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC,
    0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
    0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32,
    0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D,
    0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A, 0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38,
    0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF,
    0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
    0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1,
    0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4,
    0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F, 0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB,
    0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA,
    0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
    0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0,
    0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97,
    0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C, 0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E,
    0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89,
    0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
    0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83,
    0x41, 0x81, 0x80, 0x40
};

void radar_data_process(CircularQueue *queue) {
    static bool recvInProgress = false;
    static uint8_t ndx = 0;
    uint8_t startMarker = MESSAGE_HEAD; // Header frame (0x55)
    uint8_t dataLen = 0;
    uint8_t Msg[MAX_DATA_LEN] = {0,}; // Data buffer
    bool newData = false;

    // Process data byte by byte until we find a complete message or queue is empty
    while (!queue_is_empty(queue) && !newData) {
        uint8_t rb = queue_dequeue(queue);
        
        if(!recvInProgress) {
            // Looking for start marker
            if(rb == startMarker) {
                recvInProgress = true;
                ndx = 0; // Reset index when starting new message
            }
        }
        else {
            // Receiving message data
            if(ndx < MAX_DATA_LEN) {
                Msg[ndx] = rb;
                if(ndx == 0) dataLen = Msg[0]; // First byte is data length
                ndx++;
            }
            // Check if we have received the complete message
            else {
                recvInProgress = false;
                ndx = 0;
                newData = true;
            }
        }
    }
    
    // Process the complete message if we have one
    if(newData) {
        SleepInfer(Msg);
    }
}

//Sleep time decoding
void SleepTimeCalculate(uint8_t inf1, uint8_t inf2, uint8_t inf3, uint8_t inf4){
  uint32_t rel = 0;
  rel = (inf1 << 24) + (inf2 << 16) + (inf3 << 8) + (inf4);
  char time_str[20];
  sprintf(time_str, "%lu\r\n", rel);
  UART_Send_String(time_str);
}

//Radar transmits data frames for display via serial port
void ShowData(uint8_t inf[]){
  for (uint8_t n = 0; n < MAX_DATA_LEN; n++) {
    char hex_str[4];
    sprintf(hex_str, "%02X ", inf[n]);
    UART_Send_String(hex_str);
  }
  UART_Send_String("\r\n");
}

//Respiratory sleep data frame decoding
void SleepInfer(uint8_t* inf){
  switch(inf[3]){
    case SLEEP_INF:
#ifdef RADAR_DEBUG_MODE
      ShowData(inf);
#endif
      switch(inf[4]){
        case BREATH:
          switch(inf[5]){
            case BREATH_RATE:
              UART_Send_String("The current detected respiratory rate is: ");
              // Convert int to string and send
              char rate_str[10];
              sprintf(rate_str, "%d\r\n", inf[6]);
              UART_Send_String(rate_str);
              break;
            case CHECK_SIGN:
              switch(inf[6]){
                case BREATH_HOLD:
                  UART_Send_String("Abnormal breath-holding detected.\r\n");
                  break;
                case BREATH_NULL:
                  UART_Send_String("No detection signal at the moment.\r\n");
                  break;
                case BREATH_NORMAL:
                  UART_Send_String("Normal breathing was detected.\r\n");
                  break;
                case BREATH_MOVE:
                  UART_Send_String("Abnormal motion is detected.\r\n");
                  break;
                case BREATH_RAPID:
                  UART_Send_String("Abnormal shortness of breath was detected.\r\n");
                  break;
              }
              break;
          }
          break;
        case SCENARIO:
          switch(inf[5]){
            case CLOSE_AWAY_BED:
              switch(inf[6]){
                case AWAY_BED:
                  UART_Send_String("Detects someone leaving the bed.\r\n");
                  break;
                case CLOSE_BED:
                  UART_Send_String("Detects someone in bed.\r\n");
                  break;
              }
              break;
            case SLEEP_STATE:
              switch(inf[6]){
                case AWAKE:
                  UART_Send_String("Current user status detected: Awake.\r\n");
                  break;
                case LIGHT_SLEEP:
                  UART_Send_String("Current user status detected: Light sleep.\r\n");
                  break;
                case DEEP_SLEEP:
                  UART_Send_String("Current user status detected: Deep sleep.\r\n");
                  break;
                case SLEEP_NULL:
                  UART_Send_String("Current user status detected: NULL.\r\n");
                  break;
              }
              break;
          }
          break;
        case SLEEP_TIME:
          switch(inf[5]){
            case AWAKE_TIME:
              UART_Send_String("The user's awake time is detected as: ");
              break;
            case LIGHT_SLEEP_TIME:
              UART_Send_String("The user's light sleep time is detected as: ");
              break;
            case DEEP_SLEEP_TIME:
              UART_Send_String("The user's deep sleep time is detected as: ");
              break;
          }
          SleepTimeCalculate(inf[6], inf[7], inf[8], inf[9]);
          break;
        case SLEEP_QUALITY:
          switch(inf[5]){
            case SLEEP_SCORE:
              UART_Send_String("Judgment of sleep quality scores: ");
              char score_str[10];
              sprintf(score_str, "%d\r\n", inf[6]);
              UART_Send_String(score_str);
              break;
          }
          break;
      }
      UART_Send_String("----------------------------\r\n");
      break;
  }
}

// Function to calculate CRC16 checksum
uint16_t CalculateCrc16(unsigned char *lpuc_Frame, unsigned short int lus_Len){
  unsigned char luc_CRCHi = 0xFF;
  unsigned char luc_CRCLo = 0xFF;
  int li_Index=0;
  while(lus_Len--){
    li_Index = luc_CRCLo ^ *( lpuc_Frame++);
    luc_CRCLo = (unsigned char)( luc_CRCHi ^ cuc_CRCHi[li_Index]);
    luc_CRCHi = cuc_CRCLo[li_Index];
  }
  return (unsigned short int )(luc_CRCLo << 8 | luc_CRCHi);
}

// Function to create a checksum for the data and send to radar
void makechecksum(uint8_t* data, uint8_t length) {
    uint8_t datas[length + 2];
    for(int i = 0; i < length; i++) {
        datas[i] = data[i];
    }
    // Calculate checksum
    uint16_t crc_data = CalculateCrc16(datas, length);
    
#ifdef RADAR_DEBUG_MODE
    char buff[32];
    sprintf(buff, "The CRC16 value is : %04x \r\n", crc_data);
    UART_Send_String(buff);
#endif
    
    datas[length] = (uint8_t)((crc_data & 0xff00) >> 8); // High byte
    datas[length + 1] = (uint8_t)(crc_data & 0x00ff); // Low byte
    
#ifdef RADAR_DEBUG_MODE
    UART_Send_String("The datas send to the radar : ");
    for(int i = 0; i < length + 2; i++) {
        char buffsend[8];
        sprintf(buffsend, "0x%02X ", datas[i]);
        UART_Send_String(buffsend);
    }
    UART_Send_String("\r\n");
#endif
    
    // Send data to mmWave Radar via UART2
    send_to_radar(datas, length + 2);
}

// Function to send data to mmWave Radar via UART2
void send_to_radar(uint8_t* data, uint8_t length) {
    // Send data via UART2 to mmWave Radar using uart.c function
#ifdef RADAR_DEBUG_MODE
    HAL_StatusTypeDef status = UART2_Send_Data_WithStatus(data, length);
    if(status == HAL_OK) {
        UART_Send_String("Data sent to radar successfully\r\n");
    } else {
        UART_Send_String("Failed to send data to radar\r\n");
    }
#else
    UART2_Send_Data(data, length);
#endif
}
