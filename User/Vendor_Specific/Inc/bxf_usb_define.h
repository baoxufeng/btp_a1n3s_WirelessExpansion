#ifndef __BXF_USB_DEFINE_H
#define __BXF_USB_DEFINE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include "usbh_def.h"

typedef enum 
{
  BXF_USB_ERROR        = 0x00U,
  BXF_USB_CONNECTED    = 0x01U,
  BXF_USB_DISCONNECTED = 0x02U
} BXF_USB_StatusTypeDef;

extern BXF_USB_StatusTypeDef bxf_usb_state;

BXF_USB_StatusTypeDef BXF_USB_GetUSBState(USBH_HandleTypeDef *phost);

#ifdef __cplusplus
}
#endif

#endif /* __BXF_USB_DEFINE_H */
  
  