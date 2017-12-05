#include "stdio.h"
#include "stdint.h"
#include "stdbool.h"
#include "string.h"

//#define FOLLOW_LINE_MIN_PWM 11000
#define FOLLOW_LINE_MIN_PWM 10000

typedef struct
{
  uint8_t state;
	int16_t santikis;
	int16_t greitis;
  int16_t greitis_R;
  int16_t greitis_L;
  uint16_t delay;
  int16_t stop_pwm;
  int16_t greitis_posukiai;
  bool pwm_start_flag;	
	uint16_t delay_start;
	int16_t min_pwm;
	
}followline_t;


typedef struct
{
   uint16_t Pid_P;
   uint16_t Pid_I;
	 uint16_t Pid_D;

}followline_options_t;

#define LINE_FOLLOW_GREITIS_CONFIG_SIZE 6

typedef struct
{
	 bool    follow_pwm[LINE_FOLLOW_GREITIS_CONFIG_SIZE];
   int16_t greitis[LINE_FOLLOW_GREITIS_CONFIG_SIZE];
	 int16_t greitis_pos[LINE_FOLLOW_GREITIS_CONFIG_SIZE];
	 followline_options_t pid1[LINE_FOLLOW_GREITIS_CONFIG_SIZE];
	 followline_options_t pid2[LINE_FOLLOW_GREITIS_CONFIG_SIZE];
	 int16_t stop_pwm[LINE_FOLLOW_GREITIS_CONFIG_SIZE];

}followline_options2_t;


enum {
  FOLLOWLINE_NONE=0,
	FOLLOWLINE_DELAY_START,
	FOLLOWLINE_START,
	FOLLOWLINE_START_ENC,
	FOLLOWLINE_START_PWM,
	FOLLOWLINE_STOP,
	
};


void followline_init(void);
void follow_line_1kHz(void);

//line_falgs 1- set 0-not
//bitai XXX 4 3 2 1 0

//line error mm 0- 800 //400 vidurys 100 700krastai

void followline_handler(uint16_t line_error_mm, uint16_t line_flags);
void follow_line_config_pid(int16_t Pid_P, int16_t Pid_I, int16_t Pid_D);
void follow_line_config_pid2(int16_t Pid_P, int16_t Pid_I, int16_t Pid_D);
void follow_line_change_max_speed(int16_t speed);

void follow_line_encoder(uint16_t line_error_mm, uint16_t line_flags);
void follow_line_pwm(uint16_t line_error_mm, uint16_t line_flags);


void folow_line_stop(void); //stop komanda
//void re

void follow_line_set_pwm1(void);
void follow_line_set_pwm2(void);
void follow_line_set_pwm3(void);
void follow_line_set_pwm4(void);
void follow_line_set_pwm5(void);
void follow_line_set_pwm6(void);
void follow_line_set_pwm7(void);
void follow_line_set_pwm8(void);
void follow_line_set_pwm9(void);


void follow_line_set_pwm10(void);
void follow_line_set_pwm11(void);
void follow_line_set_pwm12(void);


void follow_line_set_pwm13(void);
void follow_line_set_pwm14(void);
void follow_line_set_pwm15(void);

void follow_line_set_pwm16(void);
void follow_line_set_pwm17(void);
void follow_line_set_pwm18(void);

void follow_line_set_pwm19(void);
void follow_line_set_pwm20(void);
void follow_line_set_pwm21(void);

void follow_line_set_pwm22(void);
void follow_line_set_pwm23(void);
void follow_line_set_pwm24(void);

