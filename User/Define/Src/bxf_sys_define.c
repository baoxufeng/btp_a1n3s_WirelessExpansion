#include "bxf_sys_define.h"

BXF_USR_Flag32BitTypeDef bxf_user_led_flag;
BXF_USR_Flag32BitTypeDef bxf_user_key_flag;
BXF_USR_Flag32BitTypeDef bxf_user_int_flag;
BXF_USR_Flag32BitTypeDef bxf_user_nrf_flag;

void BXF_USR_DefineInit(void)
{
  *(uint32_t *)&bxf_user_led_flag = 0x08U;
  *(uint32_t *)&bxf_user_key_flag = 0x00U;
  *(uint32_t *)&bxf_user_int_flag = 0x00U;
  *(uint32_t *)&bxf_user_nrf_flag = 0x00U;
}




