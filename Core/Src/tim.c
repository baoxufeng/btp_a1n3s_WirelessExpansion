/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    tim.c
  * @brief   This file provides code for the configuration
  *          of the TIM instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "tim.h"

/* USER CODE BEGIN 0 */
#include "bxf_sys.h"
#include "bxf_sys_define.h"
#include "nrf24l01p.h"
/* USER CODE END 0 */

TIM_HandleTypeDef htim11;

/* TIM11 init function */
void MX_TIM11_Init(void)
{

  /* USER CODE BEGIN TIM11_Init 0 */

  /* USER CODE END TIM11_Init 0 */

  /* USER CODE BEGIN TIM11_Init 1 */

  /* USER CODE END TIM11_Init 1 */
  htim11.Instance = TIM11;
  htim11.Init.Prescaler = 959;
  htim11.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim11.Init.Period = 999;
  htim11.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim11.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim11) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM11_Init 2 */
  HAL_TIM_Base_Start_IT(&htim11);
  /* USER CODE END TIM11_Init 2 */

}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* tim_baseHandle)
{

  if(tim_baseHandle->Instance==TIM11)
  {
  /* USER CODE BEGIN TIM11_MspInit 0 */

  /* USER CODE END TIM11_MspInit 0 */
    /* TIM11 clock enable */
    __HAL_RCC_TIM11_CLK_ENABLE();

    /* TIM11 interrupt Init */
    HAL_NVIC_SetPriority(TIM1_TRG_COM_TIM11_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(TIM1_TRG_COM_TIM11_IRQn);
  /* USER CODE BEGIN TIM11_MspInit 1 */

  /* USER CODE END TIM11_MspInit 1 */
  }
}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* tim_baseHandle)
{

  if(tim_baseHandle->Instance==TIM11)
  {
  /* USER CODE BEGIN TIM11_MspDeInit 0 */

  /* USER CODE END TIM11_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM11_CLK_DISABLE();

    /* TIM11 interrupt Deinit */
    HAL_NVIC_DisableIRQ(TIM1_TRG_COM_TIM11_IRQn);
  /* USER CODE BEGIN TIM11_MspDeInit 1 */

  /* USER CODE END TIM11_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  
  static uint8_t led_blink=0xFF, key=0xFF;
//  static uint16_t count, led_pin, led_blink_count, key_count=0x0000;
  static uint8_t send_count=0;
  uint32_t *led_flag, *key_flag;
  static GPIO_TypeDef *led_gpio;
  LED_HandleTypeDef *timled;
  
  if(htim->Instance == TIM11)
  {
    
    key_flag = (uint32_t *)(&bxf_user_key_flag);
    led_flag = (uint32_t *)(&bxf_user_led_flag);
  
//    if(*key_flag != 0x00U)
//    {
//      switch(*key_flag)
//      {
//        case 0x01U:
//          if(HAL_GPIO_ReadPin(USER_KEY_GPIO_Port, USER_KEY_Pin) == GPIO_PIN_RESET)
//            key = 0x00;
//          else
//          {  
//            *key_flag &= (0xFFFFFFFE);
//            key = 0xFF;
//            key_count = 0x0000;
//            BXF_UsrLog("User Key up!");
//          }
//          break;
//        
//        default :
//          break;
//      }
//    }
//    
//    if(key == 0x00)
//    {
//      if(key_count > 100)
//      {
//        if((key_count-100) == 5)
//        {
//          BXF_UsrLog("User Key down!");
//          key_count = 101;
//        }
//        else
//          key_count++;
//      }
//      else
//        key_count++;
//    }
      
    
//    if(*led_flag != 0x00U)
//    {
//      switch(*led_flag)
//      {
//        case 0x01U:
//          led_gpio = USER_LED_GPIO_Port;
//          led_pin = USER_LED_Pin;
//          *led_flag &= 0xFFFFFFFE;
//          timled = bled[0];
//          break;
//          
//        default :
//          break;
//      }
//      
//      switch(timled->LED_STATE)
//      {
//        case USER_LED_OFF:
//          led_blink = 0xFF;
//          HAL_GPIO_WritePin(led_gpio, led_pin, GPIO_PIN_SET);
//          break;
//        
//        case USER_LED_ON:
//          led_blink = 0xFF;
//          HAL_GPIO_WritePin(led_gpio, led_pin, GPIO_PIN_RESET);
//          break;
//       
//        default:
//          if((timled->LED_STATE > USER_LED_ON) && (timled->LED_STATE < (USER_LED_BLINK_100ms+1)))
//          {
//            led_blink = 0x00;
//            led_blink_count = timled->LED_BLINK_TIME;
//          }
//          break;
//      }
//    }
//    
//    if(led_blink == 0x00)
//    {
//      if(count > led_blink_count)
//      {
//        count = 0;
//        HAL_GPIO_TogglePin(led_gpio, led_pin);
////        printf("led\r\n");
//      }
//      else
//      {
//        count++;
//      }
//    }
    
    if(bxf_user_led_flag.flag31 == 0)
    {
      if(send_count > 100)
      {
        
        send_count = 0;
        bxf_user_led_flag.flag31 = 1;
      }
      else
      {
        send_count++;
      }
    }
  }
}
/* USER CODE END 1 */
