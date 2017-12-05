#include "stm32f1xx_hal.h"
#include "uart1.h"
#include "encoder.h"
#include "gpio_perif.h"
#include "eeprom_ext.h"

I2C_HandleTypeDef hi2c2;
static void MX_I2C2_Init(void);

uint8_t TxBuffer_EE[32];
uint8_t RxBuffer_EE[32];

void eeprom_ext_init(void)
{
   MX_I2C2_Init();

}


/* I2C1 init function */
void MX_I2C2_Init(void)
{

  hi2c2.Instance = I2C2;
  hi2c2.Init.ClockSpeed = 100000;
  hi2c2.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLED;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLED;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLED;
  HAL_I2C_Init(&hi2c2);

}


void eeprom_ext_write_block(char * source, uint8_t ee_start_adress, uint8_t source_size)
{
  uint8_t page=ee_start_adress/EE_PAGE_SIZE; //esamas page
	uint8_t page_to;
	uint8_t j=1;
	uint8_t ee_adrr_plius=0;
	for (uint8_t i=0; i<source_size;i++) //prabegam per page
	{
		TxBuffer_EE[j++]=*(source++);
		PRINTF("tx %u",TxBuffer_EE[j-1]);
		page_to=(ee_start_adress+i+1)/EE_PAGE_SIZE;
		if (page_to>page)
		{
			page=page_to;
			//jei uzsipilde page rasom
			//adresas
			TxBuffer_EE[0]=ee_start_adress+ee_adrr_plius;
			HAL_I2C_Master_Transmit(&hi2c2, I2C_ADDRESS_EE<<1, (uint8_t*)TxBuffer_EE, j, 500);
			HAL_Delay(5); //plaukiam 5ms
			ee_adrr_plius+=j-1;
		  j=1; //new data pildimas
			PRINTF("siunciam page %u adress %u\r\n",page-1,TxBuffer_EE[0]);
			
		}
	}
	if (j>1)
	{
		//jei nedapildem page pagiam rasyti
		TxBuffer_EE[0]=ee_start_adress+ee_adrr_plius;
		HAL_I2C_Master_Transmit(&hi2c2, I2C_ADDRESS_EE<<1, (uint8_t*)TxBuffer_EE, j, 500);
		//siunciam
		PRINTF("siunciam\r\n");
		HAL_Delay(5); //plaukiam 5ms
	}

}

void eeprom_ext_read_block(char * dest, uint8_t ee_start_adress, uint8_t dest_size)
{
	TxBuffer_EE[0]=ee_start_adress;
	HAL_I2C_Master_Transmit(&hi2c2, I2C_ADDRESS_EE<<1, (uint8_t*)TxBuffer_EE, 1, 100);
  HAL_I2C_Master_Receive(&hi2c2, I2C_ADDRESS_EE<<1, (uint8_t *)dest, dest_size, 100);
	//HAL_I2C_Master_Receive(&I2cHandle, I2C_ADDRESS_EE<<1, (uint8_t *)aRxBuffer_i2c, dest_size, 100);
}


