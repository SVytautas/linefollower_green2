#include "stdio.h"
#include "stdint.h"
#include "stdbool.h"
#include "string.h"


#define ADC_CNT_SIZE 6
#define ADC_SAMPLING_TIME ADC_SAMPLETIME_7CYCLES_5


//CHECK clear bit
#define SETBIT(ADDRESS,BIT) (ADDRESS |= (1<<BIT))

#define CLEARBIT(ADDRESS,BIT) (ADDRESS &= ~(1<<BIT))

#define CHECKBIT(ADDRESS,BIT) (ADDRESS & (1<<BIT))

#define BIT_FLIP(ADDRESS,BIT) ((ADDRESS)^=(BIT))

#define BIT(x) (0x01 << (x))


//bitu sudeliojimo apidrezimas
#define ADC_PERIF_SENSOR_BIT_SET_SK 2048 //puse 
#define ADC_PERIF_SENSOR_BIT_SET_HIST 100 // histerize

#define ADC_PERIF_SENSOR_BIT_SET (ADC_PERIF_SENSOR_BIT_SET_SK+ADC_PERIF_SENSOR_BIT_SET_HIST) 
#define ADC_PERIF_SENSOR_BIT_CLEAR (ADC_PERIF_SENSOR_BIT_SET_SK-ADC_PERIF_SENSOR_BIT_SET_HIST) 


//--------------Baterijo itampos matavimas---------------------------
#define ADC_PERIF_SAMPLES_BAT_CNT 3000 // ~2kHz adc matavimas kas sekunde matuojam
#define ADC_PERIF_1ADC_mV 0.002441f
//#define ADC_BATERIJA_MIN_VOLTAGE 6.4f //minimali 2 celiu itampa 3.2 celei li-po
#define ADC_BATERIJA_MIN_VOLTAGE 3.2f //minimali 2 celiu itampa 3.2 celei li-po
#define ADC_BAT_BEEP_TIME 500 // 2s pipsejimas
#define ADC_BAT_BEEP_CNT  5 // 2s pipsejimas

typedef struct
{
	uint16_t bat_voltage_adc;
	uint32_t bat_voltage_suma;
	uint16_t bat_voltage_cnt;
	float    bat_voltage;
}adc_perif_t;


#define ADC_PERIF_ADC_INS 5 //jutikliu skaicius
#define ADC_PERIF_SENSOR_NORM 12
#define ADC_PERIF_SENSOR_NORM_SHIFT 8

#define ADC_PERIF_SENSOR_DEBOUNCE 5

typedef struct
{
	uint16_t min_adc[ADC_PERIF_ADC_INS];
  uint16_t max_adc[ADC_PERIF_ADC_INS];	
}adc_perif_kalib_t;

typedef struct
{
	uint16_t 	calib_state;
	uint32_t 	calib_delay;
  bool 		 	calib_on;	
	uint32_t  sensor_coef[ADC_PERIF_ADC_INS];
	uint16_t  sensor_linear[ADC_PERIF_ADC_INS];
	uint16_t  sensor_linear_rev[ADC_PERIF_ADC_INS];
	uint16_t  sensor_err;
	uint16_t  sensor_err_last;
	uint16_t  sensor_flag_last;
	uint16_t  sensor_flag;
	uint16_t  sensor_flag_0_cnt;
	
}adc_perif_sensor_t;

enum {
ADC_PERIF_CALIB_NONE=0,
ADC_PERIF_CALIB_INIT,
ADC_PERIF_CALIBRATING,
ADC_PERIF_CALIB_END,
	
};


void adc_perif_init(void);
void adc_perif_init2(void);
void adc_perif_1kHz(void);
void adc_calib_sensor_handler(void);
void adc_perif_set_calibrating(void);
void adc_perif_print_test(void);
void adc_perif_kalib_print(void);
void adc_perif_print_test_rev(void);
void adc_perif_calc_error(void);


