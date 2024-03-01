/**
  ******************************************************************************
  * @file    usbh_VS_btp_a1n3s.c
  * @author  MCD Application Team
  * @brief   This file is the application layer for USB Host VS Keyboard handling
  *          QWERTY and AZERTY Keyboard are supported as per the selection in
  *          usbh_VS_btp_a1n3s.h
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2015 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                      www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* BSPDependencies
- "stm32xxxxx_{eval}{discovery}{nucleo_144}.c"
- "stm32xxxxx_{eval}{discovery}_io.c"
- "stm32xxxxx_{eval}{discovery}{adafruit}_lcd.c"
- "stm32xxxxx_{eval}{discovery}_sdram.c"
EndBSPDependencies */

/* Includes ------------------------------------------------------------------*/
#include "usbh_vendor_specific_btp_a1n3s.h"
#include "usbh_vendor_specific_parser.h"

/** @addtogroup USBH_LIB
  * @{
  */

/** @addtogroup USBH_CLASS
  * @{
  */

/** @addtogroup USBH_VS_CLASS
  * @{
  */

/** @defgroup USBH_VS_BTP_A1N3S
  * @brief    This file includes VS Layer Handlers for USB Host VS class.
  * @{
  */

/** @defgroup USBH_VS_BTP_A1N3S_Private_TypesDefinitions
  * @{
  */
/**
  * @}
  */


/** @defgroup USBH_VS_BTP_A1N3S_Private_Defines
  * @{
  */
/**
  * @}
  */


/** @defgroup USBH_VS_BTP_A1N3S_Private_Macros
  * @{
  */
/**
  * @}
  */

/** @defgroup USBH_VS_BTP_A1N3S_Private_FunctionPrototypes
  * @{
  */
static USBH_StatusTypeDef USBH_VS_BtpA1n3sDecode(USBH_HandleTypeDef *phost);
/**
  * @}
  */

/** @defgroup USBH_VS_BTP_A1N3S_Private_Variables
  * @{
  */

VS_BTP_A1N3S_Info_TypeDef     btp_a1n3s_info;
uint8_t                   btp_a1n3s_rx_report_buf[20];
uint8_t                   btp_a1n3s_report_data[20];
uint8_t                   btp_a1n3s_report_data_buff[20];


/**
  * @brief  USBH_VS_BtpA1n3sInit
  *         The function init the VS keyboard.
  * @param  phost: Host handle
  * @retval USBH Status
  */
USBH_StatusTypeDef USBH_VS_BtpA1n3sInit(USBH_HandleTypeDef *phost)
{
  uint32_t x;
  VS_HandleTypeDef *VS_Handle = (VS_HandleTypeDef *) phost->pActiveClass->pData;

  for (x = 0U; x < (sizeof(btp_a1n3s_report_data) / sizeof(uint32_t)); x++)
  {
    btp_a1n3s_report_data[x] = 0U;
    btp_a1n3s_rx_report_buf[x] = 0U;
  }

  if (VS_Handle->length > (sizeof(btp_a1n3s_report_data)))
  {
    VS_Handle->length = (uint16_t)(sizeof(btp_a1n3s_report_data));
  }
  VS_Handle->pData = (uint8_t *)(void *)btp_a1n3s_rx_report_buf;
  USBH_VS_FifoInit(&VS_Handle->fifo, phost->device.Data, (uint16_t)(VS_QUEUE_SIZE * sizeof(btp_a1n3s_report_data)));

  return USBH_OK;
}

/**
  * @brief  USBH_VS_GetBtpA1n3sInfo
  *         The function return keyboard information.
  * @param  phost: Host handle
  * @retval keyboard information
  */
VS_BTP_A1N3S_Info_TypeDef *USBH_VS_GetBtpA1n3sInfo(USBH_HandleTypeDef *phost)
{
  if (USBH_VS_BtpA1n3sDecode(phost) == USBH_OK)
  {
    return &btp_a1n3s_info;
      return NULL;
  }
  else
  {
    return NULL;
  }
}

/**
  * @brief  USBH_VS_BtpA1n3sDecode
  *         The function decode keyboard data.
  * @param  phost: Host handle
  * @retval USBH Status
  */
static USBH_StatusTypeDef USBH_VS_BtpA1n3sDecode(USBH_HandleTypeDef *phost)
{
  uint8_t i,f=0;
  uint8_t *pData;
  
  pData = (uint8_t *)&btp_a1n3s_info;
  
  VS_HandleTypeDef *VS_Handle = (VS_HandleTypeDef *) phost->pActiveClass->pData;
  if (VS_Handle->length == 0U)
  {
    return USBH_FAIL;
  }
  /*Fill report */
  if (USBH_VS_FifoRead(&VS_Handle->fifo, &btp_a1n3s_report_data, VS_Handle->length) ==  VS_Handle->length)
  {
    i = 12;
    while(i)
    {
      i--;
      *(pData+i) = btp_a1n3s_report_data[i+2];
    }
    
#if 0
    i = 20;
    f = 0;
    while(i != 2)
    {
      i--;
      if(btp_a1n3s_report_data[i] != 0x00)
      {
        f = 1;
      }
    }
    
    if(f)
    {
      printf("            ");
      
      i=20;  
      while(i)
      {
        i--;
        printf("  %2d ", (i));
      }
      printf("\r\nBTP 16bit : ");
      
      i=20;  
      while(i)
      {
        i--;
        printf("0x%02x ", btp_a1n3s_report_data[i]);
      }
      printf("\r\n");
    }
#endif
    return USBH_OK;
  }
  return   USBH_FAIL;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

