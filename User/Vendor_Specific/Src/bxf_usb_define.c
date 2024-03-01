#include "bxf_usb_define.h"

BXF_USB_StatusTypeDef bxf_usb_state;

BXF_USB_StatusTypeDef BXF_USB_GetUSBState(USBH_HandleTypeDef *phost)
{
  if(phost->device.is_connected != 0U)
  {
    return BXF_USB_CONNECTED;
  }
  else if(phost->device.is_connected == 0U)
  {
    return BXF_USB_DISCONNECTED;
  }
  
  return BXF_USB_ERROR;
}
