#include "stdio.h"
#include "stdint.h"
#include "stdbool.h"

#define EE_PAGE_SIZE 					16  //0-255 page
#define I2C_ADDRESS_EE        0x50 //24C04 adresas

typedef struct
{
	uint16_t bat_voltage_adc;
	uint32_t bat_voltage_suma;
	uint16_t bat_voltage_cnt;
	float    bat_voltage;
}eeprom_ext_t;


void eeprom_ext_init(void);
void eeprom_ext_handler(void);


void eeprom_ext_write_block(char * source, uint8_t ee_start_adress, uint8_t source_size);
void eeprom_ext_read_block(char * dest, uint8_t ee_start_adress, uint8_t dest_size);