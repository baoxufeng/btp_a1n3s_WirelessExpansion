#include "bxf_sys_define.h"

BXF_USR_Flag32BitTypeDef bxf_user_led_flag;
BXF_USR_Flag32BitTypeDef bxf_user_key_flag;
BXF_USR_Flag32BitTypeDef bxf_user_int_flag;
BXF_USR_Flag32BitTypeDef bxf_user_nrf_flag;
VS_BTP_A1N3S_Info_TypeDef bxf_btp_a1n3s_info;


void BXF_USR_DefineInit(void)
{
  *(uint32_t *)&bxf_user_led_flag = 0x00U;
  *(uint32_t *)&bxf_user_key_flag = 0x00U;
  *(uint32_t *)&bxf_user_int_flag = 0x00U;
  *(uint32_t *)&bxf_user_nrf_flag = 0x02U;
}

void BXF_USR_MEMCopy(uint8_t *pdata1, uint8_t *pdata2, uint16_t Size)
{
  uint16_t i;
  
  i = Size;
  while(i)
  {
    i--;
    *(pdata2+i) = *(pdata1+i);
  }
}

void BXF_USR_MEMFill(uint8_t *pdata1, uint8_t data, uint16_t Size)
{
  uint16_t i;
  
  i = Size;
  while(i)
  {
    i--;
    *(pdata1+i) = data;
  }
}
