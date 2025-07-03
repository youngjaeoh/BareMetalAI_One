#ifndef __BUZZER_H__
#define __BUZZER_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

// 부저 사용 여부를 설정하는 매크로 (부저가 연결되어 있을 때만 주석 해제)
// #define USE_BUZZER

#ifdef USE_BUZZER
#define BUZZER_GPIO_Port GPIOE
#define BUZZER_GPIO_Pin GPIO_PIN_5
#endif

// Function prototypes for buzzer control
void Buzzer_On(void);                    // Turn buzzer on
void Buzzer_Off(void);                   // Turn buzzer off
void Buzzer_Test(uint16_t duration_ms);  // Test buzzer for specified duration

#ifdef __cplusplus
}
#endif

#endif /* __BUZZER_H__ */