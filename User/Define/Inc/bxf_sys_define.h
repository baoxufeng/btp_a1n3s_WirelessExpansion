#ifndef __BXF_SYS_DEFINE_H
#define __BXF_SYS_DEFINE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

typedef struct
{
  uint32_t flag0:1;
  uint32_t flag1:1;
  uint32_t flag2:1;
  uint32_t flag3:1;
  uint32_t flag4:1;
  uint32_t flag5:1;
  uint32_t flag6:1;
  uint32_t flag7:1;
  uint32_t flag8:1;
  uint32_t flag9:1;
  uint32_t flag10:1;
  uint32_t flag11:1;
  uint32_t flag12:1;
  uint32_t flag13:1;
  uint32_t flag14:1;
  uint32_t flag15:1;
  uint32_t flag16:1;
  uint32_t flag17:1;
  uint32_t flag18:1;
  uint32_t flag19:1;
  uint32_t flag20:1;
  uint32_t flag21:1;
  uint32_t flag22:1;
  uint32_t flag23:1;
  uint32_t flag24:1;
  uint32_t flag25:1;
  uint32_t flag26:1;
  uint32_t flag27:1;
  uint32_t flag28:1;
  uint32_t flag29:1;
  uint32_t flag30:1;
  uint32_t flag31:1;
} BXF_USR_Flag32BitTypeDef;
  
extern BXF_USR_Flag32BitTypeDef bxf_user_led_flag;
extern BXF_USR_Flag32BitTypeDef bxf_user_key_flag;

void BXF_USR_DefineInit(void);

#ifdef __cplusplus
}
#endif

#endif /* __BXF_SYS_DEFINE_H */

