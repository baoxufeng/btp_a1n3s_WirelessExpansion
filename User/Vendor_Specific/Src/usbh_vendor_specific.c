/**
  ******************************************************************************
  * @file    usbh_VS.c
  * @author  MCD Application Team
  * @brief   This file is the VS Layer Handlers for USB Host VS class.
  *
  * @verbatim
  *
  *          ===================================================================
  *                                VS Class  Description
  *          ===================================================================
  *           This module manages the VS class V1.11 following the "Device Class Definition
  *           for Human Interface Devices (VS) Version 1.11 Jun 27, 2001".
  *           This driver implements the following aspects of the specification:
  *             - The Boot Interface Subclass
  *             - The Mouse and Keyboard protocols
  *
  *  @endverbatim
  *
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
#include "usbh_vendor_specific.h"
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

/** @defgroup USBH_VS_CORE
  * @brief    This file includes VS Layer Handlers for USB Host VS class.
  * @{
  */

/** @defgroup USBH_VS_CORE_Private_TypesDefinitions
  * @{
  */
/**
  * @}
  */


/** @defgroup USBH_VS_CORE_Private_Defines
  * @{
  */
/**
  * @}
  */


/** @defgroup USBH_VS_CORE_Private_Macros
  * @{
  */
/**
  * @}
  */


/** @defgroup USBH_VS_CORE_Private_Variables
  * @{
  */

/**
  * @}
  */


/** @defgroup USBH_VS_CORE_Private_FunctionPrototypes
  * @{
  */

static USBH_StatusTypeDef USBH_VS_InterfaceInit(USBH_HandleTypeDef *phost);
static USBH_StatusTypeDef USBH_VS_InterfaceDeInit(USBH_HandleTypeDef *phost);
static USBH_StatusTypeDef USBH_VS_ClassRequest(USBH_HandleTypeDef *phost);
static USBH_StatusTypeDef USBH_VS_Process(USBH_HandleTypeDef *phost);
static USBH_StatusTypeDef USBH_VS_SOFProcess(USBH_HandleTypeDef *phost);
static void  USBH_VS_ParseVSDesc(VS_DescTypeDef *desc, uint8_t *buf);

extern USBH_StatusTypeDef USBH_VS_BtpA1n3sInit(USBH_HandleTypeDef *phost);

USBH_ClassTypeDef  VS_Class =
{
  "Vendor Specific",
  USB_VS_CLASS,
  USBH_VS_InterfaceInit,
  USBH_VS_InterfaceDeInit,
  USBH_VS_ClassRequest,
  USBH_VS_Process,
  USBH_VS_SOFProcess,
  NULL,
};
/**
  * @}
  */


/** @defgroup USBH_VS_CORE_Private_Functions
  * @{
  */


/**
  * @brief  USBH_VS_InterfaceInit
  *         The function init the VS class.
  * @param  phost: Host handle
  * @retval USBH Status
  */
static USBH_StatusTypeDef USBH_VS_InterfaceInit(USBH_HandleTypeDef *phost)
{
  USBH_StatusTypeDef status;
  VS_HandleTypeDef *VS_Handle;
  uint8_t max_ep;
  uint8_t num = 0U;
  uint8_t interface;

  interface = USBH_FindInterface(phost, phost->pActiveClass->ClassCode, VS_BOOT_CODE, 0xFFU);

  if ((interface == 0xFFU) || (interface >= USBH_MAX_NUM_INTERFACES)) /* No Valid Interface */
  {
    USBH_DbgLog("Cannot Find the interface for %s class.", phost->pActiveClass->Name);
    return USBH_FAIL;
  }

  status = USBH_SelectInterface(phost, interface);

  if (status != USBH_OK)
  {
    return USBH_FAIL;
  }

  phost->pActiveClass->pData = (VS_HandleTypeDef *)USBH_malloc(sizeof(VS_HandleTypeDef));
  VS_Handle = (VS_HandleTypeDef *) phost->pActiveClass->pData;

  if (VS_Handle == NULL)
  {
    USBH_DbgLog("Cannot allocate memory for VS Handle");
    return USBH_FAIL;
  }

  /* Initialize VS handler */
  (void)USBH_memset(VS_Handle, 0, sizeof(VS_HandleTypeDef));

  VS_Handle->state = VS_ERROR;

  /*Decode Bootclass Protocol: Mouse or Keyboard*/
  if (phost->device.CfgDesc.Itf_Desc[interface].bInterfaceProtocol == VS_BTP_A1N3S_BOOT_CODE)
  {
    USBH_UsrLog("BTP-A1N3S device found!");
    VS_Handle->Init = USBH_VS_BtpA1n3sInit;
  }
  else
  {
    USBH_UsrLog("Protocol not supported.");
    return USBH_FAIL;
  }

  VS_Handle->state     = VS_INIT;
  VS_Handle->ctl_state = VS_REQ_INIT;
  VS_Handle->ep_addr   = phost->device.CfgDesc.Itf_Desc[interface].Ep_Desc[0].bEndpointAddress;
  VS_Handle->length    = phost->device.CfgDesc.Itf_Desc[interface].Ep_Desc[0].wMaxPacketSize;
  VS_Handle->poll      = phost->device.CfgDesc.Itf_Desc[interface].Ep_Desc[0].bInterval;

  if (VS_Handle->poll  < VS_MIN_POLL)
  {
    VS_Handle->poll = VS_MIN_POLL;
  }

  /* Check of available number of endpoints */
  /* Find the number of EPs in the Interface Descriptor */
  /* Choose the lower number in order not to overrun the buffer allocated */
  max_ep = ((phost->device.CfgDesc.Itf_Desc[interface].bNumEndpoints <= USBH_MAX_NUM_ENDPOINTS) ?
            phost->device.CfgDesc.Itf_Desc[interface].bNumEndpoints : USBH_MAX_NUM_ENDPOINTS);


  /* Decode endpoint IN and OUT address from interface descriptor */
  for (num = 0U; num < max_ep; num++)
  {
    if ((phost->device.CfgDesc.Itf_Desc[interface].Ep_Desc[num].bEndpointAddress & 0x80U) != 0U)
    {
      VS_Handle->InEp = (phost->device.CfgDesc.Itf_Desc[interface].Ep_Desc[num].bEndpointAddress);
      VS_Handle->InPipe = USBH_AllocPipe(phost, VS_Handle->InEp);

      /* Open pipe for IN endpoint */
      (void)USBH_OpenPipe(phost, VS_Handle->InPipe, VS_Handle->InEp, phost->device.address,
                          phost->device.speed, USB_EP_TYPE_INTR, VS_Handle->length);

      (void)USBH_LL_SetToggle(phost, VS_Handle->InPipe, 0U);
    }
    else
    {
      VS_Handle->OutEp = (phost->device.CfgDesc.Itf_Desc[interface].Ep_Desc[num].bEndpointAddress);
      VS_Handle->OutPipe  = USBH_AllocPipe(phost, VS_Handle->OutEp);

      /* Open pipe for OUT endpoint */
      (void)USBH_OpenPipe(phost, VS_Handle->OutPipe, VS_Handle->OutEp, phost->device.address,
                          phost->device.speed, USB_EP_TYPE_INTR, VS_Handle->length);

      (void)USBH_LL_SetToggle(phost, VS_Handle->OutPipe, 0U);
    }
  }

  return USBH_OK;
}

/**
  * @brief  USBH_VS_InterfaceDeInit
  *         The function DeInit the Pipes used for the VS class.
  * @param  phost: Host handle
  * @retval USBH Status
  */
static USBH_StatusTypeDef USBH_VS_InterfaceDeInit(USBH_HandleTypeDef *phost)
{
  VS_HandleTypeDef *VS_Handle = (VS_HandleTypeDef *) phost->pActiveClass->pData;

  if (VS_Handle->InPipe != 0x00U)
  {
    (void)USBH_ClosePipe(phost, VS_Handle->InPipe);
    (void)USBH_FreePipe(phost, VS_Handle->InPipe);
    VS_Handle->InPipe = 0U;     /* Reset the pipe as Free */
  }

  if (VS_Handle->OutPipe != 0x00U)
  {
    (void)USBH_ClosePipe(phost, VS_Handle->OutPipe);
    (void)USBH_FreePipe(phost, VS_Handle->OutPipe);
    VS_Handle->OutPipe = 0U;     /* Reset the pipe as Free */
  }

  if ((phost->pActiveClass->pData) != NULL)
  {
    USBH_free(phost->pActiveClass->pData);
    phost->pActiveClass->pData = 0U;
  }

  return USBH_OK;
}

/**
  * @brief  USBH_VS_ClassRequest
  *         The function is responsible for handling Standard requests
  *         for VS class.
  * @param  phost: Host handle
  * @retval USBH Status
  */
static USBH_StatusTypeDef USBH_VS_ClassRequest(USBH_HandleTypeDef *phost)
{

  USBH_StatusTypeDef status         = USBH_BUSY;
  USBH_StatusTypeDef classReqStatus = USBH_BUSY;
  VS_HandleTypeDef *VS_Handle = (VS_HandleTypeDef *) phost->pActiveClass->pData;

  /* Switch VS state machine */
  switch (VS_Handle->ctl_state)
  {
    case VS_REQ_INIT:
    case VS_REQ_GET_VS_DESC:

      USBH_VS_ParseVSDesc(&VS_Handle->VS_Desc, phost->device.CfgDesc_Raw);

      VS_Handle->ctl_state = VS_REQ_GET_REPORT_DESC;

      break;
    case VS_REQ_GET_REPORT_DESC:

      /* Get Report Desc */
      classReqStatus = USBH_VS_GetVSReportDescriptor(phost, VS_Handle->VS_Desc.wItemLength);
      if (classReqStatus == USBH_OK)
      {
        /* The descriptor is available in phost->device.Data */
        VS_Handle->ctl_state = VS_REQ_SET_IDLE;
      }
      else if (classReqStatus == USBH_NOT_SUPPORTED)
      {
        USBH_ErrLog("Control error: VS: Device Get Report Descriptor request failed");
        status = USBH_FAIL;
      }
      else
      {
        /* .. */
      }

      break;

    case VS_REQ_SET_IDLE:

      classReqStatus = USBH_VS_SetIdle(phost, 0U, 0U);

      /* set Idle */
      if (classReqStatus == USBH_OK)
      {
        VS_Handle->ctl_state = VS_REQ_SET_PROTOCOL;
      }
      else
      {
        if (classReqStatus == USBH_NOT_SUPPORTED)
        {
          VS_Handle->ctl_state = VS_REQ_SET_PROTOCOL;
        }
      }
      break;

    case VS_REQ_SET_PROTOCOL:
      /* set protocol */
      classReqStatus = USBH_VS_SetProtocol(phost, 0U);
      if (classReqStatus == USBH_OK)
      {
        VS_Handle->ctl_state = VS_REQ_IDLE;

        /* all requests performed*/
        phost->pUser(phost, HOST_USER_CLASS_ACTIVE);
        status = USBH_OK;
      }
      else if (classReqStatus == USBH_NOT_SUPPORTED)
      {
        USBH_ErrLog("Control error: VS: Device Set protocol request failed");
        status = USBH_FAIL;
      }
      else
      {
        /* .. */
      }
      break;

    case VS_REQ_IDLE:
    default:
      break;
  }

  return status;
}

/**
  * @brief  USBH_VS_Process
  *         The function is for managing state machine for VS data transfers
  * @param  phost: Host handle
  * @retval USBH Status
  */
static USBH_StatusTypeDef USBH_VS_Process(USBH_HandleTypeDef *phost)
{
  USBH_StatusTypeDef status = USBH_OK;
  VS_HandleTypeDef *VS_Handle = (VS_HandleTypeDef *) phost->pActiveClass->pData;
  uint32_t XferSize;

  switch (VS_Handle->state)
  {
    case VS_INIT:
      VS_Handle->Init(phost);
      VS_Handle->state = VS_IDLE;

#if (USBH_USE_OS == 1U)
      phost->os_msg = (uint32_t)USBH_URB_EVENT;
#if (osCMSIS < 0x20000U)
      (void)osMessagePut(phost->os_event, phost->os_msg, 0U);
#else
      (void)osMessageQueuePut(phost->os_event, &phost->os_msg, 0U, 0U);
#endif
#endif
      break;

    case VS_IDLE:
      status = USBH_VS_GetReport(phost, 0x01U, 0U, VS_Handle->pData, (uint8_t)VS_Handle->length);
      if (status == USBH_OK)
      {
        VS_Handle->state = VS_SYNC;
      }
      else if (status == USBH_BUSY)
      {
        VS_Handle->state = VS_IDLE;
        status = USBH_OK;
      }
      else if (status == USBH_NOT_SUPPORTED)
      {
        VS_Handle->state = VS_SYNC;
        status = USBH_OK;
      }
      else
      {
        VS_Handle->state = VS_ERROR;
        status = USBH_FAIL;
      }

#if (USBH_USE_OS == 1U)
      phost->os_msg = (uint32_t)USBH_URB_EVENT;
#if (osCMSIS < 0x20000U)
      (void)osMessagePut(phost->os_event, phost->os_msg, 0U);
#else
      (void)osMessageQueuePut(phost->os_event, &phost->os_msg, 0U, 0U);
#endif
#endif
      break;

    case VS_SYNC:
      /* Sync with start of Even Frame */
      if ((phost->Timer & 1U) != 0U)
      {
        VS_Handle->state = VS_GET_DATA;
      }

#if (USBH_USE_OS == 1U)
      phost->os_msg = (uint32_t)USBH_URB_EVENT;
#if (osCMSIS < 0x20000U)
      (void)osMessagePut(phost->os_event, phost->os_msg, 0U);
#else
      (void)osMessageQueuePut(phost->os_event, &phost->os_msg, 0U, 0U);
#endif
#endif
      break;

    case VS_GET_DATA:
      (void)USBH_InterruptReceiveData(phost, VS_Handle->pData,
                                      (uint8_t)VS_Handle->length,
                                      VS_Handle->InPipe);

      VS_Handle->state = VS_POLL;
      VS_Handle->timer = phost->Timer;
      VS_Handle->DataReady = 0U;
      break;

    case VS_POLL:
      if (USBH_LL_GetURBState(phost, VS_Handle->InPipe) == USBH_URB_DONE)
      {
        XferSize = USBH_LL_GetLastXferSize(phost, VS_Handle->InPipe);

        if ((VS_Handle->DataReady == 0U) && (XferSize != 0U))
        {
          (void)USBH_VS_FifoWrite(&VS_Handle->fifo, VS_Handle->pData, VS_Handle->length);
          VS_Handle->DataReady = 1U;
          USBH_VS_EventCallback(phost);

#if (USBH_USE_OS == 1U)
          phost->os_msg = (uint32_t)USBH_URB_EVENT;
#if (osCMSIS < 0x20000U)
          (void)osMessagePut(phost->os_event, phost->os_msg, 0U);
#else
          (void)osMessageQueuePut(phost->os_event, &phost->os_msg, 0U, 0U);
#endif
#endif
        }
      }
      else
      {
        /* IN Endpoint Stalled */
        if (USBH_LL_GetURBState(phost, VS_Handle->InPipe) == USBH_URB_STALL)
        {
          /* Issue Clear Feature on interrupt IN endpoint */
          if (USBH_ClrFeature(phost, VS_Handle->ep_addr) == USBH_OK)
          {
            /* Change state to issue next IN token */
            VS_Handle->state = VS_GET_DATA;
          }
        }
      }
      break;

    default:
      break;
  }

  return status;
}

/**
  * @brief  USBH_VS_SOFProcess
  *         The function is for managing the SOF Process
  * @param  phost: Host handle
  * @retval USBH Status
  */
static USBH_StatusTypeDef USBH_VS_SOFProcess(USBH_HandleTypeDef *phost)
{
  VS_HandleTypeDef *VS_Handle = (VS_HandleTypeDef *) phost->pActiveClass->pData;

  if (VS_Handle->state == VS_POLL)
  {
    if ((phost->Timer - VS_Handle->timer) >= VS_Handle->poll)
    {
      VS_Handle->state = VS_GET_DATA;

#if (USBH_USE_OS == 1U)
      phost->os_msg = (uint32_t)USBH_URB_EVENT;
#if (osCMSIS < 0x20000U)
      (void)osMessagePut(phost->os_event, phost->os_msg, 0U);
#else
      (void)osMessageQueuePut(phost->os_event, &phost->os_msg, 0U, 0U);
#endif
#endif
    }
  }
  return USBH_OK;
}

/**
  * @brief  USBH_Get_VS_ReportDescriptor
  *         Issue report Descriptor command to the device. Once the response
  *         received, parse the report descriptor and update the status.
  * @param  phost: Host handle
  * @param  Length : VS Report Descriptor Length
  * @retval USBH Status
  */
USBH_StatusTypeDef USBH_VS_GetVSReportDescriptor(USBH_HandleTypeDef *phost,
                                                   uint16_t length)
{

  USBH_StatusTypeDef status;

  status = USBH_GetDescriptor(phost,
                              USB_REQ_RECIPIENT_INTERFACE | USB_REQ_TYPE_STANDARD,
                              USB_DESC_HID_REPORT,
                              phost->device.Data,
                              length);

  /* VS report descriptor is available in phost->device.Data.
  In case of USB Boot Mode devices for In report handling ,
  VS report descriptor parsing is not required.
  In case, for supporting Non-Boot Protocol devices and output reports,
  user may parse the report descriptor*/


  return status;
}

/**
  * @brief  USBH_Get_VS_Descriptor
  *         Issue VS Descriptor command to the device. Once the response
  *         received, parse the report descriptor and update the status.
  * @param  phost: Host handle
  * @param  Length : VS Descriptor Length
  * @retval USBH Status
  */
USBH_StatusTypeDef USBH_VS_GetVSDescriptor(USBH_HandleTypeDef *phost,
                                             uint16_t length)
{
  USBH_StatusTypeDef status;

  status = USBH_GetDescriptor(phost,
                              USB_REQ_RECIPIENT_INTERFACE | USB_REQ_TYPE_STANDARD,
                              USB_DESC_HID,
                              phost->device.Data,
                              length);

  return status;
}

/**
  * @brief  USBH_Set_Idle
  *         Set Idle State.
  * @param  phost: Host handle
  * @param  duration: Duration for VS Idle request
  * @param  reportId : Targeted report ID for Set Idle request
  * @retval USBH Status
  */
USBH_StatusTypeDef USBH_VS_SetIdle(USBH_HandleTypeDef *phost,
                                    uint8_t duration,
                                    uint8_t reportId)
{

  phost->Control.setup.b.bmRequestType = USB_H2D | USB_REQ_RECIPIENT_INTERFACE | \
                                         USB_REQ_TYPE_CLASS;


  phost->Control.setup.b.bRequest = USB_VS_SET_IDLE;
  phost->Control.setup.b.wValue.w = (uint16_t)(((uint32_t)duration << 8U) | (uint32_t)reportId);

  phost->Control.setup.b.wIndex.w = 0U;
  phost->Control.setup.b.wLength.w = 0U;

  return USBH_CtlReq(phost, NULL, 0U);
}


/**
  * @brief  USBH_VS_Set_Report
  *         Issues Set Report
  * @param  phost: Host handle
  * @param  reportType  : Report type to be sent
  * @param  reportId    : Targeted report ID for Set Report request
  * @param  reportBuff  : Report Buffer
  * @param  reportLen   : Length of data report to be send
  * @retval USBH Status
  */
USBH_StatusTypeDef USBH_VS_SetReport(USBH_HandleTypeDef *phost,
                                      uint8_t reportType,
                                      uint8_t reportId,
                                      uint8_t *reportBuff,
                                      uint8_t reportLen)
{

  phost->Control.setup.b.bmRequestType = USB_H2D | USB_REQ_RECIPIENT_INTERFACE | \
                                         USB_REQ_TYPE_CLASS;


  phost->Control.setup.b.bRequest = USB_VS_SET_REPORT;
  phost->Control.setup.b.wValue.w = (uint16_t)(((uint32_t)reportType << 8U) | (uint32_t)reportId);

  phost->Control.setup.b.wIndex.w = 0U;
  phost->Control.setup.b.wLength.w = reportLen;

  return USBH_CtlReq(phost, reportBuff, (uint16_t)reportLen);
}


/**
  * @brief  USBH_VS_GetReport
  *         retrieve Set Report
  * @param  phost: Host handle
  * @param  reportType  : Report type to be sent
  * @param  reportId    : Targeted report ID for Set Report request
  * @param  reportBuff  : Report Buffer
  * @param  reportLen   : Length of data report to be send
  * @retval USBH Status
  */
USBH_StatusTypeDef USBH_VS_GetReport(USBH_HandleTypeDef *phost,
                                      uint8_t reportType,
                                      uint8_t reportId,
                                      uint8_t *reportBuff,
                                      uint8_t reportLen)
{

  phost->Control.setup.b.bmRequestType = USB_D2H | USB_REQ_RECIPIENT_INTERFACE | \
                                         USB_REQ_TYPE_CLASS;


  phost->Control.setup.b.bRequest = USB_VS_GET_REPORT;
  phost->Control.setup.b.wValue.w = (uint16_t)(((uint32_t)reportType << 8U) | (uint32_t)reportId);

  phost->Control.setup.b.wIndex.w = 0U;
  phost->Control.setup.b.wLength.w = reportLen;

  return USBH_CtlReq(phost, reportBuff, (uint16_t)reportLen);
}

/**
  * @brief  USBH_Set_Protocol
  *         Set protocol State.
  * @param  phost: Host handle
  * @param  protocol : Set Protocol for VS : boot/report protocol
  * @retval USBH Status
  */
USBH_StatusTypeDef USBH_VS_SetProtocol(USBH_HandleTypeDef *phost,
                                        uint8_t protocol)
{
  phost->Control.setup.b.bmRequestType = USB_H2D | USB_REQ_RECIPIENT_INTERFACE
                                         | USB_REQ_TYPE_CLASS;

  phost->Control.setup.b.bRequest = USB_VS_SET_PROTOCOL;
  if (protocol != 0U)
  {
    phost->Control.setup.b.wValue.w = 0U;
  }
  else
  {
    phost->Control.setup.b.wValue.w = 1U;
  }

  phost->Control.setup.b.wIndex.w = 0U;
  phost->Control.setup.b.wLength.w = 0U;

  return USBH_CtlReq(phost, NULL, 0U);

}

/**
  * @brief  USBH_ParseVSDesc
  *         This function Parse the VS descriptor
  * @param  desc: VS Descriptor
  * @param  buf: Buffer where the source descriptor is available
  * @retval None
  */
static void  USBH_VS_ParseVSDesc(VS_DescTypeDef *desc, uint8_t *buf)
{
  USBH_DescHeader_t *pdesc = (USBH_DescHeader_t *)buf;
  uint16_t CfgDescLen;
  uint16_t ptr;

  CfgDescLen = LE16(buf + 2U);

  if (CfgDescLen > USB_CONFIGURATION_DESC_SIZE)
  {
    ptr = USB_LEN_CFG_DESC;

    while (ptr < CfgDescLen)
    {
      pdesc = USBH_GetNextDesc((uint8_t *)pdesc, &ptr);

      if (pdesc->bDescriptorType == USB_DESC_TYPE_HID)
      {
        desc->bLength = *(uint8_t *)((uint8_t *)pdesc + 0U);
        desc->bDescriptorType = *(uint8_t *)((uint8_t *)pdesc + 1U);
        desc->bcdVS = LE16((uint8_t *)pdesc + 2U);
        desc->bCountryCode = *(uint8_t *)((uint8_t *)pdesc + 4U);
        desc->bNumDescriptors = *(uint8_t *)((uint8_t *)pdesc + 5U);
        desc->bReportDescriptorType = *(uint8_t *)((uint8_t *)pdesc + 6U);
        desc->wItemLength = LE16((uint8_t *)pdesc + 7U);
        break;
      }
    }
  }
}

/**
  * @brief  USBH_VS_GetDeviceType
  *         Return Device function.
  * @param  phost: Host handle
  * @retval VS function: VS_MOUSE / VS_KEYBOARD
  */
VS_TypeTypeDef USBH_VS_GetDeviceType(USBH_HandleTypeDef *phost)
{
  VS_TypeTypeDef   type = VS_UNKNOWN;
  uint8_t InterfaceProtocol;

  if (phost->gState == HOST_CLASS)
  {
    InterfaceProtocol = phost->device.CfgDesc.Itf_Desc[phost->device.current_interface].bInterfaceProtocol;
    if (InterfaceProtocol == VS_BTP_A1N3S_BOOT_CODE)
    {
      type = VS_BTP_A1N3S;
    }
  }
  return type;
}


/**
  * @brief  USBH_VS_GetPollInterval
  *         Return VS device poll time
  * @param  phost: Host handle
  * @retval poll time (ms)
  */
uint8_t USBH_VS_GetPollInterval(USBH_HandleTypeDef *phost)
{
  VS_HandleTypeDef *VS_Handle = (VS_HandleTypeDef *) phost->pActiveClass->pData;

  if ((phost->gState == HOST_CLASS_REQUEST) ||
      (phost->gState == HOST_INPUT) ||
      (phost->gState == HOST_SET_CONFIGURATION) ||
      (phost->gState == HOST_CHECK_CLASS) ||
      ((phost->gState == HOST_CLASS)))
  {
    return (uint8_t)(VS_Handle->poll);
  }
  else
  {
    return 0U;
  }
}
/**
  * @brief  USBH_VS_FifoInit
  *         Initialize FIFO.
  * @param  f: Fifo address
  * @param  buf: Fifo buffer
  * @param  size: Fifo Size
  * @retval none
  */
void USBH_VS_FifoInit(FIFO_VS_TypeDef *f, uint8_t *buf, uint16_t size)
{
  f->head = 0U;
  f->tail = 0U;
  f->lock = 0U;
  f->size = size;
  f->buf = buf;
}

/**
  * @brief  USBH_VS_FifoRead
  *         Read from FIFO.
  * @param  f: Fifo address
  * @param  buf: read buffer
  * @param  nbytes: number of item to read
  * @retval number of read items
  */
uint16_t USBH_VS_FifoRead(FIFO_VS_TypeDef *f, void *buf, uint16_t nbytes)
{
  uint16_t i;
  uint8_t *p;

  p = (uint8_t *) buf;

  if (f->lock == 0U)
  {
    f->lock = 1U;

    for (i = 0U; i < nbytes; i++)
    {
      if (f->tail != f->head)
      {
        *p++ = f->buf[f->tail];
        f->tail++;

        if (f->tail == f->size)
        {
          f->tail = 0U;
        }
      }
      else
      {
        f->lock = 0U;
        return i;
      }
    }
  }

  f->lock = 0U;

  return nbytes;
}

/**
  * @brief  USBH_VS_FifoWrite
  *         Write To FIFO.
  * @param  f: Fifo address
  * @param  buf: read buffer
  * @param  nbytes: number of item to write
  * @retval number of written items
  */
uint16_t USBH_VS_FifoWrite(FIFO_VS_TypeDef *f, void *buf, uint16_t  nbytes)
{
  uint16_t i;
  uint8_t *p;

  p = (uint8_t *) buf;

  if (f->lock == 0U)
  {
    f->lock = 1U;

    for (i = 0U; i < nbytes; i++)
    {
      if (((f->head + 1U) == f->tail) ||
          (((f->head + 1U) == f->size) && (f->tail == 0U)))
      {
        f->lock = 0U;
        return i;
      }
      else
      {
        f->buf[f->head] = *p++;
        f->head++;

        if (f->head == f->size)
        {
          f->head = 0U;
        }
      }
    }
  }

  f->lock = 0U;

  return nbytes;
}

/**
  * @brief  The function is a callback about VS Data events
  *  @param  phost: Selected device
  * @retval None
  */
__weak void USBH_VS_EventCallback(USBH_HandleTypeDef *phost)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(phost);
}
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


/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
