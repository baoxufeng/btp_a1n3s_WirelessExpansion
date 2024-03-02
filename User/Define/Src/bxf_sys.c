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
      led_flag = 0x01U;
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
    (*(uint32_t *)&bxf_user_led_flag) = led_flag;
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

void BXF_USR_KEY_Proess(void)
{
  if(bxf_user_int_flag.flag0 == 1)
  {
    
    bxf_user_key_flag.flag0 = 1;
    bxf_user_int_flag.flag0 = 0;
  }
}

void BXF_USR_TIM11_Proess(void)
{
  static uint8_t key_1s=200,led_blink=0xFF;
  static uint16_t led_pin,led_blink_count,count;
  uint32_t *led_flag;
  static GPIO_TypeDef *led_gpio;
  LED_HandleTypeDef *timled;
  
  if(bxf_user_int_flag.flag2 == 1) //10ms
  {
    if(bxf_user_key_flag.flag0 == 1)
    {
      if(HAL_GPIO_ReadPin(USER_KEY_GPIO_Port, USER_KEY_Pin) == GPIO_PIN_SET)
      {
        key_1s = 200;
        bxf_user_key_flag.flag0 = 0;
        BXF_UsrLog("user key up!");
      }
      else if((HAL_GPIO_ReadPin(USER_KEY_GPIO_Port, USER_KEY_Pin) == GPIO_PIN_RESET) && (key_1s == 200))
      {
        key_1s = 0;
        BXF_UsrLog("user key down!");
      }
    }
    
    if((*(uint32_t *)&bxf_user_led_flag) != 0)
    {
      led_flag = (uint32_t *)(&bxf_user_led_flag);
      switch(*led_flag)
      {
        case 0x01U:
          led_gpio = USER_LED_GPIO_Port;
          led_pin = USER_LED_Pin;
          *led_flag &= 0xFFFFFFFE;
          timled = bled[0];
          break;
        
        default :
          break;
      }
    }
    
    switch(timled->LED_STATE)
    {
      case USER_LED_OFF:
        led_blink = 0xFF;
        HAL_GPIO_WritePin(led_gpio, led_pin, GPIO_PIN_SET);
        break;
      
      case USER_LED_ON:
        led_blink = 0xFF;
        HAL_GPIO_WritePin(led_gpio, led_pin, GPIO_PIN_RESET);
        break;
     
      default:
        if((timled->LED_STATE > USER_LED_ON) && (timled->LED_STATE < (USER_LED_BLINK_100ms+1)))
        {
          led_blink = 0x00;
          led_blink_count = timled->LED_BLINK_TIME;
        }
        break;
    }
  
  
    if(led_blink == 0x00)
    {
      if(count > led_blink_count)
      {
        count = 0;
        HAL_GPIO_TogglePin(led_gpio, led_pin);
      }
      else
      {
        count++;
      }
    }
    
    bxf_user_int_flag.flag2 = 0;
  }
  
  if(bxf_user_int_flag.flag3 == 1) //100ms
  {
    if(key_1s < bxf_key_delay)
    {
      key_1s++;
    }
    else if(key_1s < 200)
    {
      key_1s = bxf_key_delay;
      BXF_UsrLog("user key down!");
    }
    bxf_user_int_flag.flag3 = 0;
  }
}







