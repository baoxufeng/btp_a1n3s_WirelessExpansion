#include "nrf24l01p.h"
#include "bxf_sys_define.h"
#include "bxf_sys.h"

uint8_t nrf_tx_addr[5]={0x00,0x1A,0xB1,0xB1,0x01};
uint8_t nrf_rx_addr[5]={0x00,0x1A,0xB1,0xB1,0x01};
uint8_t nrf_rx_buff[nrf_rx_buff_width];

void BXF_NRF_WriteData(uint8_t BXF_nRF24L01P_REG, uint8_t REGData)
{
  uint8_t state;
  nRF24L01P_CS(0);
  
  nRF24L01P_SPI_T_CMD(&BXF_nRF24L01P_REG, &state, 1);
//  BXF_UsrLog("state = %#2x ", state);
  nRF24L01P_SPI_T(&REGData, 1);
  
  nRF24L01P_CS(1);
}  

void BXF_NRF_WriteBuff(uint8_t BXF_nRF24L01P_REG, uint8_t *REGDataBuff, uint8_t Size)
{
  uint8_t state;
  nRF24L01P_CS(0);
  
  nRF24L01P_SPI_T_CMD(&BXF_nRF24L01P_REG, &state, 1);
//  BXF_UsrLog("state = %#2x ", state);
  nRF24L01P_SPI_T(REGDataBuff, Size);
  
  nRF24L01P_CS(1);
}

uint8_t BXF_NRF_ReadData(uint8_t BXF_nRF24L01P_REG)
{
  uint8_t reg_data, state;
  
  nRF24L01P_CS(0);
  
  nRF24L01P_SPI_T_CMD(&BXF_nRF24L01P_REG, &state, 1);
//  BXF_UsrLog("state = %#2x ", state);
  nRF24L01P_SPI_R(&reg_data, 1);
  
  nRF24L01P_CS(1);
  
  return reg_data;
}

void BXF_NRF_ReadBuff(uint8_t BXF_nRF24L01P_REG, uint8_t *REGDataBuff, uint8_t Size)
{
  uint8_t state;
  nRF24L01P_CS(0);
  
  nRF24L01P_SPI_T_CMD(&BXF_nRF24L01P_REG, &state, 1);
//  BXF_UsrLog("state = %#2x ", state);
  nRF24L01P_SPI_R(REGDataBuff, Size);
  
  nRF24L01P_CS(1);
}



void BXF_NRF_TxMode(void)
{
  uint8_t i, RX_buff[5];
  
//  i=5;
//  while(i)
//  {
//    i--;
//    nrf_tx_addr[i] = 0x7E;
//  }
  
  nRF24L01P_CE(0);
  
  BXF_NRF_WriteBuff(nRF24L01P_W_reg(NRF_REG_TX_ADDR),    nrf_tx_addr, 5);
  
  BXF_NRF_ReadBuff(nRF24L01P_R_reg(NRF_REG_TX_ADDR), RX_buff, 5);

  BXF_UsrLog("NRF_REG_TX_ADDR Data");
  for(i=0;i<5;i++)
  {
    printf("RX_buff[%d] = %#2x ", i, RX_buff[i]);
  }
    printf("\r\n");
    
  BXF_NRF_WriteBuff(nRF24L01P_W_reg(NRF_REG_RX_ADDR_P0), nrf_tx_addr, 5);
  
  
  
  BXF_NRF_WriteData(nRF24L01P_W_reg(NRF_REG_EN_AA),    0x01);
  BXF_NRF_WriteData(nRF24L01P_W_reg(NRF_REG_ENRXADDR), 0x01);
  
  BXF_NRF_WriteData(nRF24L01P_W_reg(NRF_REG_SETUP_RETR), 0x1A);
  BXF_NRF_WriteData(nRF24L01P_W_reg(NRF_REG_RF_CH),      0x28);
  BXF_NRF_WriteData(nRF24L01P_W_reg(NRF_REG_RF_SETUP),   0x0F);
  BXF_NRF_WriteData(nRF24L01P_W_reg(NRF_REG_CONFIG),     0x0E);
  
}

BXF_NRF_StatusTypeDef BXF_NRF_TxDate(uint8_t *pData, uint8_t Size)
{
  uint8_t state;
  uint8_t nrf_tx_data[32];
  
  while(bxf_user_nrf_flag.flag1 == 0);
  
  nrf_tx_data[0] = Size;
  
  
  BXF_USR_MEMCopy(pData, &(nrf_tx_data[1]), Size);
//  while((bxf_user_led_flag.flag3 == 0) ||  (bxf_user_led_flag.flag2 == 1));
  
  nRF24L01P_CE(0);
  BXF_NRF_WriteBuff(NRF_CMD_W_TX_PAYLOAD, nrf_tx_data, 32);
//  HAL_Delay(1);
  bxf_user_nrf_flag.flag1 = 0;
  nRF24L01P_CE(1);
  
  return (BXF_NRF_OK);
}

void BXF_NRF_RxMode(void)
{
  uint8_t i;
  
//  i=5;
//  while(i)
//  {
//    i--;
//    nrf_rx_addr[i] = 0x7E;
//  }
  
  nRF24L01P_CE(0);
  
  BXF_NRF_WriteBuff(nRF24L01P_W_reg(NRF_REG_RX_ADDR_P0), nrf_rx_addr, 5);
  
  BXF_NRF_WriteData(nRF24L01P_W_reg(NRF_REG_EN_AA),    0x01);
  BXF_NRF_WriteData(nRF24L01P_W_reg(NRF_REG_ENRXADDR), 0x01);
  
  BXF_NRF_WriteData(nRF24L01P_W_reg(NRF_REG_RF_CH),      0x28);
  BXF_NRF_WriteData(nRF24L01P_W_reg(NRF_REG_RX_PW_P0),   nrf_rx_buff_width);
  BXF_NRF_WriteData(nRF24L01P_W_reg(NRF_REG_RF_SETUP),   0x0F);
  BXF_NRF_WriteData(nRF24L01P_W_reg(NRF_REG_CONFIG),     0x0F);
  
  nRF24L01P_CE(1);
}  

BXF_NRF_StatusTypeDef NRF24L01P_Check(void) 
{
    uint8_t TX_buff[5]={0xC2,0xC2,0xC2,0xC2,0xC2}, RX_buff[5]={0};
    uint8_t i;
    
    /* Write an address of 5 bytes */
    BXF_NRF_WriteBuff(nRF24L01P_W_reg(NRF_REG_TX_ADDR), TX_buff, 5);
    
    HAL_Delay(500);
    
    /* Read the written address */
    BXF_NRF_ReadBuff(nRF24L01P_R_reg(NRF_REG_TX_ADDR), RX_buff, 5);
    
    BXF_UsrLog("NRF_REG_TX_ADDR Data");
    for(i=0;i<5;i++)
    {
      printf("RX_buff[%d] = %#2x ", i, RX_buff[i]);
    }
      printf("\r\n");
    
    for(i=0;i<5;i++) {
    
        
        if(RX_buff[i] != TX_buff[i])
            return (BXF_NRF_ERROR); //Communication failed, returned 0
    }
    
    return (BXF_NRF_OK); //Communication successful, return 1
}

void BXF_NRF_nRF24L01P_Init(void)
{
  uint8_t i, state, inin_data[31];
  
  HAL_Delay(150);
  
  nRF24L01P_CS(1);
  
  BXF_USR_MEMFill(inin_data, 0xAA, 31);
  
  BXF_USR_MEMFill(nrf_rx_buff, 0x00, nrf_rx_buff_width);
  
  while(NRF24L01P_Check() == BXF_NRF_ERROR)
  {
    BXF_ErrLog("nRF24L01P SPI communication failure!");
    HAL_Delay(500);
  }

    BXF_UsrLog("nRF24L01P SPI communication successful!");
  
    state = BXF_NRF_ReadData(nRF24L01P_R_reg(NRF_REG_STATUS));
    BXF_NRF_WriteData(nRF24L01P_W_reg(NRF_REG_STATUS), state);
  
  #ifdef nRF24L01P_TX
    BXF_NRF_TxMode();
    
    BXF_NRF_TxDate(inin_data, 31);
  #endif
  
  #ifdef nRF24L01P_RX
    BXF_NRF_RxMode();
  #endif
}



