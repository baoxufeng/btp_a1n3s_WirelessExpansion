/**
  ******************************************************************************
  * @file    usbh_hid_keybd.h
  * @author  MCD Application Team
  * @brief   This file contains all the prototypes for the usbh_hid_keybd.c
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

/* Define to prevent recursive -----------------------------------------------*/
#ifndef __USBH_VENDOR_SPECIFIC_BTP_A1N3S_H
#define __USBH_VENDOR_SPECIFIC_BTP_A1N3S_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "usbh_vendor_specific.h"
#include "usbh_vendor_specific_btp_a1n3s.h"

/** @addtogroup USBH_LIB
  * @{
  */

/** @addtogroup USBH_CLASS
  * @{
  */

/** @addtogroup USBH_VS_CLASS
  * @{
  */

/** @defgroup USBH_VS_KEYBD
  * @brief This file is the Header file for usbh_hid_keybd.c
  * @{
  */


/** @defgroup USBH_VS_KEYBD_Exported_Types
  * @{
  */
  
typedef struct _VS_BTP_A1N3S_Info
{
  uint8_t              B_U:1;       //------bit |7     |6     |5     |4     |3     |2     |1     | 0
  uint8_t              B_D:1;       //  |       -----------------------------------------------------
  uint8_t              B_L:1;       //  |       |RS    |LS    |back  |start |right |left  |down  |up
  uint8_t              B_R:1;       //  | '1' : active
  uint8_t              B_START:1;   //  | 1 byte
  uint8_t              B_BACK:1;    //  |
  uint8_t              B_LS:1;      //  |
  uint8_t              B_RS:1;      //---
  uint8_t              B_LB:1;      //------bit |7     |6     |5     |4     |3     |2     |1     | 0
  uint8_t              B_RB:1;      //  |       -----------------------------------------------------
  uint8_t               :2;         //  |       |Y     |X     |B     |A     |unkown       |RB    |LB
  uint8_t              B_A:1;       //  | '1' : active
  uint8_t              B_B:1;       //  | 1 byte
  uint8_t              B_X:1;       //  |
  uint8_t              B_Y:1;       //---
  uint8_t              LT;          //------ LT 0-255 1 byte
  uint8_t              RT;          //------ RT 0-255 1 byte
  int16_t              LS_X;        //------ LS X axis : -32,768-32,767 2 bytes
  int16_t              LS_Y;        //------ LS Y axis : -32,768-32,767 2 bytes
  int16_t              RS_X;        //------ RS X axis : -32,768-32,767 2 bytes
  int16_t              RS_Y;        //------ RS X axis : -32,768-32,767 2 bytes
}
VS_BTP_A1N3S_Info_TypeDef; // 12 bytes

USBH_StatusTypeDef USBH_VS_BtpA1n3sInit(USBH_HandleTypeDef *phost);
VS_BTP_A1N3S_Info_TypeDef *USBH_VS_GetBtpA1n3sInfo(USBH_HandleTypeDef *phost);
uint8_t USBH_VS_GetASCIICode(VS_BTP_A1N3S_Info_TypeDef *info);

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __USBH_VS_KEYBD_H */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

