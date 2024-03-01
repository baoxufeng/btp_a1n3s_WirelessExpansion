/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file            : usb_host.c
  * @version         : v1.0_Cube
  * @brief           : This file implements the USB Host
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

#include "usb_host.h"
#include "usbh_core.h"
#include "usbh_hid.h"

/* USER CODE BEGIN Includes */
#include "usbh_vendor_specific.h"
#include "bxf_usb_define.h"
/* USER CODE END Includes */

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USB Host core handle declaration */
USBH_HandleTypeDef hUsbHostFS;
ApplicationTypeDef Appli_state = APPLICATION_IDLE;

/*
 * -- Insert your variables declaration here --
 */
/* USER CODE BEGIN 0 */
void USBH_VS_EventCallback(USBH_HandleTypeDef *phost)
{
//  uint8_t i;
//  uint8_t* pdata;
  VS_TypeTypeDef   type = VS_UNKNOWN;
  VS_BTP_A1N3S_Info_TypeDef *user_btp_a1n3s_info;

//  USBH_UsrLog("USBH_VS_EventCallback");

  type = USBH_VS_GetDeviceType(phost);

  switch(type)
  {
    case (VS_BTP_A1N3S):
    {
//      USBH_UsrLog("type = BTP-A1N3S");
      
      user_btp_a1n3s_info = USBH_VS_GetBtpA1n3sInfo(phost);
      
      #if 1
      if(user_btp_a1n3s_info->B_U) USBH_UsrLog("button up");
      if(user_btp_a1n3s_info->B_D) USBH_UsrLog("button down");
      if(user_btp_a1n3s_info->B_L) USBH_UsrLog("button left");
      if(user_btp_a1n3s_info->B_R) USBH_UsrLog("button right");
      
      if(user_btp_a1n3s_info->B_A) USBH_UsrLog("button A");
      if(user_btp_a1n3s_info->B_B) USBH_UsrLog("button B");
      if(user_btp_a1n3s_info->B_X) USBH_UsrLog("button X");
      if(user_btp_a1n3s_info->B_Y) USBH_UsrLog("button Y");
      
      if(user_btp_a1n3s_info->B_LB) USBH_UsrLog("button LB");
      if(user_btp_a1n3s_info->B_RB) USBH_UsrLog("button RB");
      if(user_btp_a1n3s_info->B_LS) USBH_UsrLog("button LS");
      if(user_btp_a1n3s_info->B_RS) USBH_UsrLog("button RS");
      
      if(user_btp_a1n3s_info->B_START) USBH_UsrLog("button start");
      if(user_btp_a1n3s_info->B_BACK) USBH_UsrLog("button back");
      
      if(user_btp_a1n3s_info->LT) USBH_UsrLog("LT : %3d", user_btp_a1n3s_info->LT);
      if(user_btp_a1n3s_info->RT) USBH_UsrLog("RT : %3d", user_btp_a1n3s_info->RT);
      
      if((user_btp_a1n3s_info->LS_X) || (user_btp_a1n3s_info->LS_Y)) 
      {
        USBH_UsrLog("LS x : %6d, LS y : %6d", user_btp_a1n3s_info->LS_X, user_btp_a1n3s_info->LS_Y);
      }
      
      if((user_btp_a1n3s_info->RS_X) || (user_btp_a1n3s_info->RS_Y)) 
      {
        USBH_UsrLog("RS x : %6d, RS y : %6d", user_btp_a1n3s_info->RS_X, user_btp_a1n3s_info->RS_Y);
      }
    #endif
    }
    break;

    default:
    break;
  }  
}

void USBH_HID_EventCallback(USBH_HandleTypeDef *phost)
{
  HID_TypeTypeDef   type = HID_UNKNOWN;

  USBH_UsrLog("USBH_HID_EventCallback");

  type = USBH_HID_GetDeviceType(phost);

  switch(type)
  {
    case (HID_MOUSE):
    {
      USBH_UsrLog("tpye = HID_MOUSE");
      
      HID_MOUSE_Info_TypeDef* mouse_info;
      
      mouse_info = USBH_HID_GetMouseInfo(phost);
      USBH_UsrLog("mouse_info X%d, Y%d, button %d %d %d", mouse_info->x, mouse_info->y, mouse_info->buttons[2], mouse_info->buttons[1], mouse_info->buttons[0]);
    }
    break;
      
    default:
    break;
  }
}
/* USER CODE END 0 */

/*
 * user callback declaration
 */
static void USBH_UserProcess(USBH_HandleTypeDef *phost, uint8_t id);

/*
 * -- Insert your external function declaration here --
 */
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/**
  * Init USB host library, add supported class and start the library
  * @retval None
  */
void MX_USB_HOST_Init(void)
{
  /* USER CODE BEGIN USB_HOST_Init_PreTreatment */

  /* USER CODE END USB_HOST_Init_PreTreatment */

  /* Init host Library, add supported class and start the library. */
  if (USBH_Init(&hUsbHostFS, USBH_UserProcess, HOST_FS) != USBH_OK)
  {
    Error_Handler();
  }
  if (USBH_RegisterClass(&hUsbHostFS, USBH_HID_CLASS) != USBH_OK)
  {
    Error_Handler();
  }
  if (USBH_RegisterClass(&hUsbHostFS, USBH_VS_CLASS) != USBH_OK)
  {
    Error_Handler();
  }
  if (USBH_Start(&hUsbHostFS) != USBH_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USB_HOST_Init_PostTreatment */

  /* USER CODE END USB_HOST_Init_PostTreatment */
}

/*
 * Background task
 */
void MX_USB_HOST_Process(void)
{
  /* USB Host Background task */
  USBH_Process(&hUsbHostFS);
}
/*
 * user callback definition
 */
static void USBH_UserProcess  (USBH_HandleTypeDef *phost, uint8_t id)
{
  /* USER CODE BEGIN CALL_BACK_1 */
  switch(id)
  {
  case HOST_USER_SELECT_CONFIGURATION:
  break;

  case HOST_USER_DISCONNECTION:
  Appli_state = APPLICATION_DISCONNECT;
  
  break;

  case HOST_USER_CLASS_ACTIVE:
  Appli_state = APPLICATION_READY;
  break;

  case HOST_USER_CONNECTION:
  Appli_state = APPLICATION_START;
 
  break;

  default:
  break;
  }
  /* USER CODE END CALL_BACK_1 */
}

/**
  * @}
  */

/**
  * @}
  */

