#include "bxf_sys.h"
#include "bxf_sys_define.h"
#include "nrf24l01p.h"

LED_HandleTypeDef *bled[32];
LED_HandleTypeDef bled0;




HAL_StatusTypeDef BXF_USR_LEDCtrl(LED_HandleTypeDef *bled, BXF_LED_StateTypeDef BXF_LED_State, BXF_LED_CommandPriorityTypeDef  BXF_LED_CommandPriority)
{
  uint32_t led_flag=0x00U;
  
  switch(bled->LED)
  {
    case USER_LED0:
      led_flag |= 0x01U;
      break;
    
    default:
      break;
  }
  
//  if(BXF_LED_CommandPriority == Priority_NO)
//  {
//    bled->LED_CommandPriority = Priority_NO;
//    bled->LED_STATE = USER_LED_OFF;
//    (*(uint32_t *)&bxf_user_led_flag) |= led_flag;
//    return HAL_OK;
//  }
//  else if(BXF_LED_CommandPriority > bled->LED_CommandPriority)
  {
    bled->LED_CommandPriority = BXF_LED_CommandPriority;
    bled->LED_STATE = BXF_LED_State;
    
    switch(BXF_LED_State)
    {
      case USER_LED_BLINK_100ms:
        bled->LED_BLINK_TIME = 10;
        break;
      
      case USER_LED_BLINK_250ms:
        bled->LED_BLINK_TIME = 25;
        break;
        
      case USER_LED_BLINK_1s:
        bled->LED_BLINK_TIME = 100;
        break;
      
      default :
        bled->LED_BLINK_TIME = 0;
        break;
    }
    (*(uint32_t *)&bxf_user_led_flag) |= led_flag;
    return HAL_OK;
  }
//  else
//    return HAL_ERROR;
  
  return HAL_OK;
}

void BXF_USR_LEDInit(void)
{
  bled[0] = &bled0;
  
  bled0.LED = USER_LED0;
  bled0.LED_STATE = USER_LED_OFF;
  bled0.LED_CommandPriority = Priority_NO;
  
//  (*(uint32_t *)&bxf_user_led_flag) = 0x00U;
  
  BXF_USR_LEDCtrl(&bled0, USER_LED_BLINK_1s, Priority_1);
}

void BXF_USR_SYSInit(void)
{
  BXF_USR_DefineInit();
  
  BXF_USR_LEDInit();
  BXF_NRF_nRF24L01P_Init();
}

