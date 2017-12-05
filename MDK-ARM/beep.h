#include "stdio.h"
#include "stdint.h"
#include "stdbool.h"


#define BEEP_TOGGLE		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_4);
#define BEEP_OFF			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);


typedef struct
{
	 bool enable;
	 uint16_t beep_time_ms; //50/50
	 uint16_t beep_kartai;
	 int16_t  beep_time_cnt;
   uint16_t beep_kartai_cnt;
   	
	 
}beep_t;


void beep_handler_1kHz(void);
bool beep_set_kartai(uint16_t laikas_ms, uint16_t kartai);