#ifndef __NRF24L01P_H
#define __NRF24L01P_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include "spi.h"

#define nRF24L01P_TX

#define nRF24L01P_SPI hspi1
#define nRF24L01P_CS(__AA__) \
        HAL_GPIO_WritePin(nRF24_nCS_GPIO_Port, nRF24_nCS_Pin, __AA__)
        
#define nRF24L01P_CE(__AA__) \
        HAL_GPIO_WritePin(nRF24_CE_GPIO_Port, nRF24_CE_Pin, __AA__)
        
#define nRF24L01P_SPI_T_CMD(__PTDATA__, __PRDATA__, __SIZE__) \
        HAL_SPI_TransmitReceive(&nRF24L01P_SPI, __PTDATA__, __PRDATA__, __SIZE__, 100)
        
#define nRF24L01P_SPI_T(__PDATA__, __SIZE__) \
        HAL_SPI_Transmit(&nRF24L01P_SPI, __PDATA__, __SIZE__, 100)
        
#define nRF24L01P_SPI_R(__PDATA__, __SIZE__) \
        HAL_SPI_Receive(&nRF24L01P_SPI, __PDATA__, __SIZE__, 100)

#define nRF24L01P_R_reg(__AA__) \
        (NRF_CMD_R_REGISTER | __AA__)
#define nRF24L01P_W_reg(__AA__) \
        (NRF_CMD_W_REGISTER | __AA__)

#define NRF_REG_CONFIG        0x00
#define NRF_REG_EN_AA         0x01
#define NRF_REG_ENRXADDR      0x02
#define NRF_REG_SETUP_AW      0x03
#define NRF_REG_SETUP_RETR    0x04
#define NRF_REG_RF_CH         0x05
#define NRF_REG_RF_SETUP      0x06
#define NRF_REG_STATUS        0x07
#define NRF_REG_OBSERVE_TX    0x08
#define NRF_REG_RPD           0x09
#define NRF_REG_RX_ADDR_P0    0x0A
#define NRF_REG_RX_ADDR_P1    0x0B
#define NRF_REG_RX_ADDR_P2    0x0C
#define NRF_REG_RX_ADDR_P3    0x0D
#define NRF_REG_RX_ADDR_P4    0x0E
#define NRF_REG_RX_ADDR_P5    0x0F
#define NRF_REG_TX_ADDR       0x10
#define NRF_REG_RX_PW_P0      0x11
#define NRF_REG_RX_PW_P1      0x12
#define NRF_REG_RX_PW_P2      0x13
#define NRF_REG_RX_PW_P3      0x14
#define NRF_REG_RX_PW_P4      0x15
#define NRF_REG_RX_PW_P5      0x16
#define NRF_REG_FIFO_STATUS   0x17
#define NRF_REG_DYNPD         0x1C
#define NRF_REG_FEATURE       0x1D

#define NRF_CMD_R_REGISTER          0x00
#define NRF_CMD_W_REGISTER          0x20
#define NRF_CMD_R_RX_PAYLOAD        0x61
#define NRF_CMD_W_TX_PAYLOAD        0xA0
#define NRF_CMD_FLUSH_TX            0xE1
#define NRF_CMD_FLUSH_RX            0xE2
#define NRF_CMD_REUSE_TX_PL         0xE3
#define NRF_CMD_R_RX_PL_WID         0x60
#define NRF_CMD_W_ACK_PAYLOAD       0xA8
#define NRF_CMD_W_TX_PAYLOAD_NOACK  0xB0
#define NRF_CMD_NOP                 0xFF

typedef enum 
{
  BXF_NRF_ERROR    = 0x00,
  BXF_NRF_OK       = 0x80,
  BXF_NRF_RX_DS    = 0x40,
  BXF_NRF_TX_DS    = 0x20,
  BXF_NRF_MAX_RT      = 0x10
} BXF_NRF_StatusTypeDef;

#define nrf_rx_buff_width 32

extern uint8_t nrf_tx_addr[5], nrf_rx_addr[5], nrf_rx_buff[nrf_rx_buff_width];

void BXF_NRF_WriteData(uint8_t BXF_nRF24L01P_REG, uint8_t REGData);
void BXF_NRF_WriteBuff(uint8_t BXF_nRF24L01P_REG, uint8_t *REGDataBuff, uint8_t Size);
uint8_t BXF_NRF_ReadData(uint8_t BXF_nRF24L01P_REG);
void BXF_NRF_ReadBuff(uint8_t BXF_nRF24L01P_REG, uint8_t *REGDataBuff, uint8_t Size);

void BXF_NRF_TxMode(void);
BXF_NRF_StatusTypeDef BXF_NRF_TxDate(uint8_t *pData);
void BXF_NRF_RxMode(void);
BXF_NRF_StatusTypeDef BXF_NRF_RxDate(uint8_t *pData);

BXF_NRF_StatusTypeDef NRF24L01P_Check(void);
void BXF_NRF_nRF24L01P_Init(void);




#ifdef __cplusplus
}
#endif

#endif /* __NRF24L01P_H */




