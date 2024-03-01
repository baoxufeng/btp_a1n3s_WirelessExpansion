/**
  ******************************************************************************
  * @file    usbh_vs.h
  * @author  MCD Application Team
  * @brief   This file contains all the prototypes for the usbh_vs.c
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

/* Define to prevent recursive  ----------------------------------------------*/
#ifndef __USBH_VENDOR_SPECIFIC_H
#define __USBH_VENDOR_SPECIFIC_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "usbh_core.h"
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

/** @defgroup USBH_VS_CORE
  * @brief This file is the Header file for usbh_vs.c
  * @{
  */


/** @defgroup USBH_VS_CORE_Exported_Types
  * @{
  */

#define VS_MIN_POLL                                10U
#define VS_REPORT_SIZE                             16U
#define VS_MAX_USAGE                               10U
#define VS_MAX_NBR_REPORT_FMT                      10U
#define VS_QUEUE_SIZE                              10U

#define  VS_ITEM_LONG                              0xFEU

#define  VS_ITEM_TYPE_MAIN                         0x00U
#define  VS_ITEM_TYPE_GLOBAL                       0x01U
#define  VS_ITEM_TYPE_LOCAL                        0x02U
#define  VS_ITEM_TYPE_RESERVED                     0x03U


#define  VS_MAIN_ITEM_TAG_INPUT                    0x08U
#define  VS_MAIN_ITEM_TAG_OUTPUT                   0x09U
#define  VS_MAIN_ITEM_TAG_COLLECTION               0x0AU
#define  VS_MAIN_ITEM_TAG_FEATURE                  0x0BU
#define  VS_MAIN_ITEM_TAG_ENDCOLLECTION            0x0CU


#define  VS_GLOBAL_ITEM_TAG_USAGE_PAGE             0x00U
#define  VS_GLOBAL_ITEM_TAG_LOG_MIN                0x01U
#define  VS_GLOBAL_ITEM_TAG_LOG_MAX                0x02U
#define  VS_GLOBAL_ITEM_TAG_PHY_MIN                0x03U
#define  VS_GLOBAL_ITEM_TAG_PHY_MAX                0x04U
#define  VS_GLOBAL_ITEM_TAG_UNIT_EXPONENT          0x05U
#define  VS_GLOBAL_ITEM_TAG_UNIT                   0x06U
#define  VS_GLOBAL_ITEM_TAG_REPORT_SIZE            0x07U
#define  VS_GLOBAL_ITEM_TAG_REPORT_ID              0x08U
#define  VS_GLOBAL_ITEM_TAG_REPORT_COUNT           0x09U
#define  VS_GLOBAL_ITEM_TAG_PUSH                   0x0AU
#define  VS_GLOBAL_ITEM_TAG_POP                    0x0BU


#define  VS_LOCAL_ITEM_TAG_USAGE                   0x00U
#define  VS_LOCAL_ITEM_TAG_USAGE_MIN               0x01U
#define  VS_LOCAL_ITEM_TAG_USAGE_MAX               0x02U
#define  VS_LOCAL_ITEM_TAG_DESIGNATOR_INDEX        0x03U
#define  VS_LOCAL_ITEM_TAG_DESIGNATOR_MIN          0x04U
#define  VS_LOCAL_ITEM_TAG_DESIGNATOR_MAX          0x05U
#define  VS_LOCAL_ITEM_TAG_STRING_INDEX            0x07U
#define  VS_LOCAL_ITEM_TAG_STRING_MIN              0x08U
#define  VS_LOCAL_ITEM_TAG_STRING_MAX              0x09U
#define  VS_LOCAL_ITEM_TAG_DELIMITER               0x0AU


/* States for VS State Machine */
typedef enum
{
  VS_INIT = 0,
  VS_IDLE,
  VS_SEND_DATA,
  VS_BUSY,
  VS_GET_DATA,
  VS_SYNC,
  VS_POLL,
  VS_ERROR,
}
VS_StateTypeDef;

typedef enum
{
  VS_REQ_INIT = 0,
  VS_REQ_IDLE,
  VS_REQ_GET_REPORT_DESC,
  VS_REQ_GET_VS_DESC,
  VS_REQ_SET_IDLE,
  VS_REQ_SET_PROTOCOL,
  VS_REQ_SET_REPORT,

}
VS_CtlStateTypeDef;

typedef enum
{
  VS_BTP_A1N3S   = 0x01,
  VS_UNKNOWN = 0xFF,
}
VS_TypeTypeDef;


typedef  struct  _VS_ReportData
{
  uint8_t   ReportID;
  uint8_t   ReportType;
  uint16_t  UsagePage;
  uint32_t  Usage[VS_MAX_USAGE];
  uint32_t  NbrUsage;
  uint32_t  UsageMin;
  uint32_t  UsageMax;
  int32_t   LogMin;
  int32_t   LogMax;
  int32_t   PhyMin;
  int32_t   PhyMax;
  int32_t   UnitExp;
  uint32_t  Unit;
  uint32_t  ReportSize;
  uint32_t  ReportCnt;
  uint32_t  Flag;
  uint32_t  PhyUsage;
  uint32_t  AppUsage;
  uint32_t  LogUsage;
}
VS_ReportDataTypeDef;

typedef  struct  _VS_ReportIDTypeDef
{
  uint8_t  Size;         /* Report size return by the device id            */
  uint8_t  ReportID;     /* Report Id                                      */
  uint8_t  Type;         /* Report Type (INPUT/OUTPUT/FEATURE)             */
} VS_ReportIDTypeDef;

typedef struct  _VS_CollectionTypeDef
{
  uint32_t                       Usage;
  uint8_t                        Type;
  struct _VS_CollectionTypeDef  *NextPtr;
} VS_CollectionTypeDef;


typedef  struct  _VS_AppCollectionTypeDef
{
  uint32_t               Usage;
  uint8_t                Type;
  uint8_t                NbrReportFmt;
  VS_ReportDataTypeDef  ReportData[VS_MAX_NBR_REPORT_FMT];
} VS_AppCollectionTypeDef;


typedef struct _VSDescriptor
{
  uint8_t   bLength;
  uint8_t   bDescriptorType;
  uint16_t  bcdVS;               /* indicates what endpoint this descriptor is describing */
  uint8_t   bCountryCode;        /* specifies the transfer type. */
  uint8_t   bNumDescriptors;     /* specifies the transfer type. */
  uint8_t   bReportDescriptorType;    /* Maximum Packet Size this endpoint is capable of sending or receiving */
  uint16_t  wItemLength;          /* is used to specify the polling interval of certain transfers. */
}
VS_DescTypeDef;


typedef struct
{
  uint8_t  *buf;
  uint16_t  head;
  uint16_t tail;
  uint16_t size;
  uint8_t  lock;
} FIFO_VS_TypeDef;


/* Structure for VS process */
typedef struct _VS_Process
{
  uint8_t              OutPipe;
  uint8_t              InPipe;
  VS_StateTypeDef     state;
  uint8_t              OutEp;
  uint8_t              InEp;
  VS_CtlStateTypeDef  ctl_state;
  FIFO_VS_TypeDef         fifo;
  uint8_t              *pData;
  uint16_t             length;
  uint8_t              ep_addr;
  uint16_t             poll;
  uint32_t             timer;
  uint8_t              DataReady;
  VS_DescTypeDef      VS_Desc;
  USBH_StatusTypeDef(* Init)(USBH_HandleTypeDef *phost);
}
VS_HandleTypeDef;

/**
  * @}
  */

/** @defgroup USBH_VS_CORE_Exported_Defines
  * @{
  */

#define USB_VS_GET_REPORT                            0x01U
#define USB_VS_GET_IDLE                              0x02U
#define USB_VS_GET_PROTOCOL                          0x03U
#define USB_VS_SET_REPORT                            0x09U
#define USB_VS_SET_IDLE                              0x0AU
#define USB_VS_SET_PROTOCOL                          0x0BU




/* VS Class Codes */
#define USB_VS_CLASS                                 0xFFU

/* Interface Descriptor field values for VS Boot Protocol */
#define VS_BOOT_CODE                                 0x5DU
#define VS_BTP_A1N3S_BOOT_CODE                       0x01U


/**
  * @}
  */

/** @defgroup USBH_VS_CORE_Exported_Macros
  * @{
  */
/**
  * @}
  */

/** @defgroup USBH_VS_CORE_Exported_Variables
  * @{
  */
extern USBH_ClassTypeDef  VS_Class;
#define USBH_VS_CLASS    &VS_Class
/**
  * @}
  */

/** @defgroup USBH_VS_CORE_Exported_FunctionsPrototype
  * @{
  */

USBH_StatusTypeDef USBH_VS_SetReport(USBH_HandleTypeDef *phost,
                                      uint8_t reportType,
                                      uint8_t reportId,
                                      uint8_t *reportBuff,
                                      uint8_t reportLen);

USBH_StatusTypeDef USBH_VS_GetReport(USBH_HandleTypeDef *phost,
                                      uint8_t reportType,
                                      uint8_t reportId,
                                      uint8_t *reportBuff,
                                      uint8_t reportLen);

USBH_StatusTypeDef USBH_VS_GetVSReportDescriptor(USBH_HandleTypeDef *phost,
                                                   uint16_t length);

USBH_StatusTypeDef USBH_VS_GetVSDescriptor(USBH_HandleTypeDef *phost,
                                             uint16_t length);

USBH_StatusTypeDef USBH_VS_SetIdle(USBH_HandleTypeDef *phost,
                                    uint8_t duration,
                                    uint8_t reportId);

USBH_StatusTypeDef USBH_VS_SetProtocol(USBH_HandleTypeDef *phost,
                                        uint8_t protocol);

void USBH_VS_EventCallback(USBH_HandleTypeDef *phost);

VS_TypeTypeDef USBH_VS_GetDeviceType(USBH_HandleTypeDef *phost);

uint8_t USBH_VS_GetPollInterval(USBH_HandleTypeDef *phost);

void USBH_VS_FifoInit(FIFO_VS_TypeDef *f, uint8_t *buf, uint16_t size);

uint16_t  USBH_VS_FifoRead(FIFO_VS_TypeDef *f, void *buf, uint16_t  nbytes);

uint16_t  USBH_VS_FifoWrite(FIFO_VS_TypeDef *f, void *buf, uint16_t nbytes);

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __USBH_VS_H */

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

