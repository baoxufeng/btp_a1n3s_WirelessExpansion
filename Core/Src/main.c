/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "main.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "usb_host.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "bxf_sys.h"
#include "bxf_sys_define.h"
#include "nrf24l01p.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_USB_HOST_Process(void);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	uint8_t TX_Data[8]; 
  uint8_t i,tx_data_tim[32]={0}, state;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
//  MX_USB_HOST_Init();
  MX_USART1_UART_Init();
  MX_TIM11_Init();
  MX_SPI1_Init();
//  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
	BXF_FWLog(__CHIP__, __VS__);
  MX_USB_HOST_Init();
  BXF_USR_SYSInit();
  
  BXF_UsrLog("System initialization is complete !");
  tx_data_tim[0] = 31;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    BXF_USR_KEY_Proess();
    BXF_USR_TIM11_Proess();
    /* USER CODE END WHILE */
    MX_USB_HOST_Process();
    
//    if(bxf_user_led_flag.flag6 == 1)
//    {
//      bxf_user_led_flag.flag6 = 0;
//      BXF_UsrLog("nRF_INT_Pin");
//      state = BXF_NRF_ReadData(nRF24L01P_R_reg(NRF_REG_STATUS));
//      BXF_NRF_WriteData(nRF24L01P_W_reg(NRF_REG_STATUS), state);

//      if(state&BXF_NRF_MAX_RT)
//      {
//        BXF_UsrLog("BXF_NRF_MAX_RT");
//        bxf_user_led_flag.flag2 = 1;
//        BXF_NRF_WriteData(NRF_CMD_FLUSH_TX, 0xFF);
//        
//      }
//      else if(state&BXF_NRF_TX_DS)
//      {
//        BXF_UsrLog("BXF_NRF_TX_DS");
//        bxf_user_led_flag.flag3 = 1;
//      }
//      else if(state&BXF_NRF_RX_DS)
//      {
//        BXF_UsrLog("BXF_NRF_RX_DS");
//        if(bxf_user_led_flag.flag4 == 0)
//        {
//          bxf_user_led_flag.flag4 = 1;
//          BXF_NRF_ReadBuff(NRF_CMD_R_RX_PAYLOAD, nrf_rx_buff, nrf_rx_buff_width);
//          BXF_NRF_WriteData(NRF_CMD_FLUSH_RX, 0xFF);
//          
//          for(i=0;i<nrf_rx_buff_width;i++)
//          {
//            printf("RX_buff[%d] = %#2x\r\n", i, nrf_rx_buff[i]);
//          }
//          bxf_user_led_flag.flag4 = 0;
//        }
//      }
//    }
//    
//    if((bxf_user_led_flag.flag31 == 1) && (bxf_user_led_flag.flag3 == 1))
//    {
//      bxf_user_led_flag.flag31 = 0;
//      BXF_UsrLog("send data!");
//      if(BXF_NRF_TxDate(tx_data_tim) == BXF_NRF_OK)
//      {
//        BXF_UsrLog("ok");
//        BXF_UsrLog("%#2x", tx_data_tim[1]);
//      }
//      else
//      {
//        BXF_ErrLog("fail");
//      }
//      
//      for(i=1;i<32;i++)
//      {
//        tx_data_tim[i]++;
//      }
//      
//    }

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 12;
  RCC_OscInitStruct.PLL.PLLN = 96;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
