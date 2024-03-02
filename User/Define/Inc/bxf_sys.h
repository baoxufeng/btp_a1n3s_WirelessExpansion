#ifndef __BXF_SYS_H
#define __BXF_SYS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

#define BXF_DEFINE_PROJECT "nRF24L01P_TX"

#define  BXF_FWLog(__AA__,__BB__)   do { \
                            printf("Chip : %s\r\n",__AA__); \
                            printf("Project : %s\r\n",BXF_DEFINE_PROJECT); \
                            printf("Firmware version : %s\r\n",__BB__); \
                            printf("Compile time : %s %s",__DATE__ ,__TIME__); \
                            printf("\r\n"); \
} while (0)

#define  BXF_UsrLog(...)   do { \
                            printf("%s line %d > BXFLog : ",__FILE__, __LINE__);\
                            printf(__VA_ARGS__); \
                            printf("\r\n"); \
} while (0)

#define  BXF_ErrLog(...)   do { \
                            printf("%s line %d > ERROR : ",__FILE__, __LINE__);\
                            printf(__VA_ARGS__); \
                            printf("\r\n"); \
} while (0)

typedef enum 
{
  USER_LED0       = 0x00,
  USER_LED1       = 0x01
} BXF_LED_NumTypeDef;

typedef enum 
{
  USER_LED_OFF      = 0x00,
  USER_LED_ON       = 0x01,
  USER_LED_BLINK_250ms = 0x02,
  USER_LED_BLINK_1s    = 0x03,
  USER_LED_BLINK_100ms = 0x04
} BXF_LED_StateTypeDef;

typedef enum 
{
  Priority_NO = 0x00U,
  Priority_1  = 0x01U,
  Priority_2  = 0x02U,
  Priority_3  = 0x03U
} BXF_LED_CommandPriorityTypeDef;

typedef struct
{
  BXF_LED_NumTypeDef                LED;
  BXF_LED_StateTypeDef              LED_STATE;
  BXF_LED_CommandPriorityTypeDef    LED_CommandPriority;
  uint16_t                          LED_BLINK_TIME;
  uint16_t                          LED_UPDATE;
} LED_HandleTypeDef;

extern LED_HandleTypeDef *bled[32];
extern LED_HandleTypeDef bled0;



HAL_StatusTypeDef BXF_USR_LEDCtrl(LED_HandleTypeDef *bled, BXF_LED_StateTypeDef BXF_LED_State, BXF_LED_CommandPriorityTypeDef  BXF_LED_CommandPriority);
void BXF_USR_LEDInit(void);
void BXF_USR_SYSInit(void);
void BXF_USR_KEY_Proess(void);
void BXF_USR_TIM11_Proess(void);


#ifdef __cplusplus
}
#endif

#endif /* __BXF_SYS_H */

