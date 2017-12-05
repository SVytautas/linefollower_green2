#include "stdio.h"
#include "stdint.h"
#include "stdbool.h"


//#define MOTOR_R_REV()		  { HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET); HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_SET);}
//#define MOTOR_R_FOR()			{ HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET); HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_RESET);}
//#define MOTOR_R_FOR()		  { HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET); HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_SET);}
//#define MOTOR_R_REV()			{ HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET); HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_RESET);}
//#define MOTOR_R_STOP()			{ HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET); HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_RESET);}

//#define MOTOR_L_REV()		  { HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_RESET); HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);}
//#define MOTOR_L_FOR()			{ HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_SET); HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);}
//#define MOTOR_L_FOR()		  { HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_RESET); HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);}
//#define MOTOR_L_REV()			{ HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_SET); HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);}
//#define MOTOR_L_STOP()			{ HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_RESET); HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);}

#define MOTOR_SPEED_TIME_UPDATE 10//10//40 //40ms kada updatinam pid

//#define MOTOR_MAX_SPEED 70 //maksimalus gretitis pagal encoderi
#define MOTOR_MAX_SPEED 48 //maksimalus gretitis pagal encoderi

#define MOTOR_PID_SPEED_CNT_RESET 10 //maksimalus gretitis pagal encoderi

typedef struct
{
	uint16_t state;
	uint16_t update_speed_cnt;
	int16_t motorR_speed;
	int16_t motorL_speed;
	bool    enable;
	
	bool     lock_to_much;
	uint16_t cnt_R_0;
	uint16_t cnt_L_0;
	
	uint16_t clean_wheals_delay;
}motor_perif_t;


void motor_perif_init(void);
void motor_perif_handler(void);
void motor_perif_tick_1khz(void);
void set_motorR_speed(int16_t motor_speed);
void set_motorL_speed(int16_t motor_speed);

//PWM valdymas
void set_motorL_speed_pwm(int16_t motor_speed_pwm);
void set_motorR_speed_pwm(int16_t motor_speed_pwm);
void set_clean_wheels(void);