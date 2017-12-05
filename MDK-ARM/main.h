#include "stdio.h"
#include "stdint.h"


//SAUGOMU STRUKTURU ADRESAI

#define MAIN_EE_ADRESS_ADC_PREIF_KALIB 0
#define MAIN_EE_ADRESS_FOLLOWLINE_OPTIONS (MAIN_EE_ADRESS_ADC_PREIF_KALIB+sizeof(adc_perif_kalib_t))



void system_tick_1khz(void);