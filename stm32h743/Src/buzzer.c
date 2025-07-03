#include "buzzer.h"
#include "main.h"

/**
  * @brief  Buzzer를 활성화 (소리 켜기) 합니다. (Active-Low 방식)
  * @param  None
  * @retval None
  */
void Buzzer_On(void)
{
#ifdef USE_BUZZER
  HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_GPIO_Pin, GPIO_PIN_RESET); // PE5를 LOW로 설정하여 부저 활성화
#endif
}

/**
  * @brief  Buzzer를 비활성화 (소리 끄기) 합니다.
  * @param  None
  * @retval None
  */
void Buzzer_Off(void)
{
#ifdef USE_BUZZER
  HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_GPIO_Pin, GPIO_PIN_SET);   // PE5를 HIGH로 설정하여 부저 비활성화
#endif
}

/**
  * @brief  Buzzer 테스트 함수 - 지정된 시간 동안 부저를 울립니다.
  * @param  duration_ms: 부저를 울릴 시간 (밀리초)
  * @retval None
  */
void Buzzer_Test(uint16_t duration_ms)
{
#ifdef USE_BUZZER
  Buzzer_On();
  HAL_Delay(duration_ms);
  Buzzer_Off();
#endif
}