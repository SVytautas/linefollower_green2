#include "stdio.h"
#include "stdint.h"
#include "stdbool.h"

#define GPIO_BTN_DEBOUNCE 10  //100hz skaitimas
#define GPIO_BTN_LONG 120  //100hz skaitimas 2sekundes

#define GPIO_BTN_STAICIUS 3

//#define GPIO_BTN_STM_READ() 				(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_8))
#define GPIO_BTN_STM_READ() 				(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_9))
#define GPIO_BTN_LEFT_READ() 				(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12))
#define GPIO_BTN_RIGHT_READ() 			(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_12))


//LEDAI------------------------------
#define GPIO_LED_RED_ON();				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_SET);
#define GPIO_LED_RED_OFF();				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_RESET);
#define GPIO_LED_RED_TOGGLE();		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_15);

#define GPIO_LED_BLUE_ON();				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13,  GPIO_PIN_SET);
#define GPIO_LED_BLUE_OFF();			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13,  GPIO_PIN_RESET);
#define GPIO_LED_BLUE_TOGGLE();		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);

//#define GPIO_LED_BLUE3_ON();				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5,  GPIO_PIN_SET);
//#define GPIO_LED_BLUE3_OFF();				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5,  GPIO_PIN_RESET);
//#define GPIO_LED_BLUE3_TOGGLE();		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_5);

//#define GPIO_LED_BLUE1_ON();				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1,  GPIO_PIN_SET);
//#define GPIO_LED_BLUE1_OFF();				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1,  GPIO_PIN_RESET);
//#define GPIO_LED_BLUE1_TOGGLE();		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_1);


//#define GPIO_LED_BLUE2_ON();				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_SET);
//#define GPIO_LED_BLUE2_OFF();				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_RESET);
//#define GPIO_LED_BLUE2_TOGGLE();		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_11);


#define GPIO_LED_GREEN_ON();				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_SET);
#define GPIO_LED_GREEN_OFF();				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_RESET);
#define GPIO_LED_GREEN_TOGGLE();		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_14);

//LED raudoni adc IR
//#define GPIO_LED_RED_SENSOR_ON();		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6,  GPIO_PIN_SET);
//#define GPIO_LED_RED_SENSOR_OFF();	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6,  GPIO_PIN_RESET);
//#define GPIO_LED_RED_SENSOR_TOGGLE();		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_6);

#define GPIO_LED_RED_SENSOR_ON();		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13,  GPIO_PIN_SET);
#define GPIO_LED_RED_SENSOR_OFF();	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13,  GPIO_PIN_RESET);
#define GPIO_LED_RED_SENSOR_TOGGLE();		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_13);


typedef struct
{
  uint32_t btn_count[GPIO_BTN_STAICIUS];
	uint32_t btn_count_long[GPIO_BTN_STAICIUS];
  bool 		 btn_status_[GPIO_BTN_STAICIUS];
  bool     btn_event[GPIO_BTN_STAICIUS];	
}gpio_btn_t;


enum {
GPIO_BTN_STM_PTR=0,  //counterio pionteris
GPIO_BTN_LEFT_PTR,
GPIO_BTN_RIGHT_PTR,

};


enum {
GPIO_BTN_STM_SHORT=0,  //counterio pionteris
GPIO_BTN_LEFT_SHORT,
GPIO_BTN_RIGHT_SHORT,
GPIO_BTN_STM_LONG,
GPIO_BTN_LEFT_LONG,
GPIO_BTN_RIGHT_LONG,

};




void gpio_perif_init(void);
void gpio_button_handler_100Hz(void);
void gpio_btn_event_handler(uint8_t btn_event);