
#include "stm32f1xx_hal.h"
#include "uart1.h"
#include "encoder.h"
#include "gpio_perif.h"
#include "beep.h"
#include "main.h"
#include "eeprom_ext.h"
#include "followline.h"
#include "adc_perif.h"
#include "pid.h"
#include "data_visualizer_print.h"
#include "motor_perif.h"


followline_t followline=
{
  .state =  FOLLOWLINE_NONE,
	.greitis = 20, //70 max
	//.greitis = 250, //70 max
	.stop_pwm = -3000,
};


followline_options_t followline_options=
{
   .Pid_P=0,
   .Pid_I=0,
	 .Pid_D=0,

};

followline_options_t followline_options2=
{
   .Pid_P=0,
   .Pid_I=0,
	 .Pid_D=0,

};

pidData_t pid_follow;
pidData_t pid_follow2;

void followline_init(void)
{

  followline.state = FOLLOWLINE_NONE;
  
	PRINTF("LINE PID P:%u I:%u D:%u",followline_options.Pid_P,
	followline_options.Pid_I,
	followline_options.Pid_D);
	
	followline_options.Pid_P=100;
	followline_options.Pid_I=0;
	followline_options.Pid_D=500;
	
	followline_options2.Pid_P=200;
	followline_options2.Pid_I=0;
	followline_options2.Pid_D=200;
	
	//linijos sekimo pid
	pid_Init(followline_options.Pid_P, followline_options.Pid_I, followline_options.Pid_D, &pid_follow);
	pid_Init(followline_options2.Pid_P, followline_options2.Pid_I, followline_options2.Pid_D, &pid_follow2);
	//followline.state=FOLLOWLINE_DELAY_START;
	followline.delay = 4000;
	//followline.delay = 100;
}


//linijos sekimo handleris

void followline_handler(uint16_t line_error_mm, uint16_t line_flags)
{

    switch (followline.state)
		{
		
		  case FOLLOWLINE_NONE:
				//nieko nedarom
			break;
			
			case FOLLOWLINE_DELAY_START:
				//nieko nedarom
			if (followline.delay)
			{
			   followline.delay--;
			}
			else
			{
			   //followline.state=FOLLOWLINE_START;
				 followline.state=FOLLOWLINE_START_PWM;
			}
			break;
			
			
			case FOLLOWLINE_START:
					
			    //initinam parametrus ir pradedam linijos sekima
			if (!followline.delay_start)
			{
			
			  if (followline.pwm_start_flag)
				{
				  followline.state=FOLLOWLINE_START_PWM;
					pid_Reset_Integrator(&pid_follow);
					pid_Reset_Integrator(&pid_follow2);						
				
				}
				else
				{
				  followline.state=FOLLOWLINE_START_ENC;
					pid_Reset_Integrator(&pid_follow);	
					pid_Reset_Integrator(&pid_follow2);	
				}
				GPIO_LED_RED_SENSOR_ON();
				GPIO_LED_RED_OFF();
				GPIO_LED_BLUE_OFF();
				GPIO_LED_GREEN_OFF();
			}				
			
			
			break;
			
			case FOLLOWLINE_START_ENC:
				  follow_line_encoder(line_error_mm,line_flags);
			break;
			
			
			case FOLLOWLINE_START_PWM:
					follow_line_pwm(line_error_mm,line_flags);
			break;
			
			case FOLLOWLINE_STOP:
					set_motorR_speed(0);
			    set_motorL_speed(0);
			    set_motorR_speed_pwm(0);
			    set_motorL_speed_pwm(0);
          GPIO_LED_RED_SENSOR_OFF();
          GPIO_LED_RED_ON();
				  GPIO_LED_BLUE_OFF();
				  GPIO_LED_GREEN_OFF();	
          if (!followline.delay_start)
					{
					   followline.state=FOLLOWLINE_NONE;
					}						
			break;
		
		  
			
			default:
				followline.state=FOLLOWLINE_NONE;
			break;
		
		
		}


}


void follow_line_config_pid(int16_t Pid_P, int16_t Pid_I, int16_t Pid_D)
{

    followline_options.Pid_P=Pid_P;
		followline_options.Pid_I=Pid_I;
		followline_options.Pid_D=Pid_D;
	
	  pid_Init(followline_options.Pid_P, followline_options.Pid_I, followline_options.Pid_D, &pid_follow);
    pid_Reset_Integrator(&pid_follow);	
    //eeprom_ext_write_block((char *)&followline_options,MAIN_EE_ADRESS_FOLLOWLINE_OPTIONS,sizeof(followline_options));
}

void follow_line_config_pid2(int16_t Pid_P, int16_t Pid_I, int16_t Pid_D)
{

    followline_options2.Pid_P=Pid_P;
		followline_options2.Pid_I=Pid_I;
		followline_options2.Pid_D=Pid_D;
	
	  pid_Init(followline_options2.Pid_P, followline_options2.Pid_I, followline_options2.Pid_D, &pid_follow2);
    pid_Reset_Integrator(&pid_follow);	
    //eeprom_ext_write_block((char *)&followline_options,MAIN_EE_ADRESS_FOLLOWLINE_OPTIONS,sizeof(followline_options));
}

void follow_line_change_max_speed(int16_t speed)
{
   followline.greitis =speed;
}

void follow_line_change_posukiai_max(int16_t speed)
{
	followline.greitis_posukiai = speed;
   
}


void follow_line_encoder(uint16_t line_error_mm, uint16_t line_flags)
{
   //nieko nedarom
			//					166   333   500   666   833
			if (line_error_mm==1 || line_error_mm==1000)
			{
			   //pid_Reset_Integrator(&pid_follow);
				followline.santikis = pid_Controller(line_error_mm, 500, &pid_follow2);
				
			}
			else
			{
			   followline.santikis = pid_Controller(line_error_mm, 500, &pid_follow);
			}
					
			//data_visualizer_print(followline.santikis,line_error_mm,400);
			//mazinam greiti vienam varikiui kad suktusi i reikiama
			if (followline.santikis>0)
			{
				//					166   333   500   666   833
				 if (line_error_mm<666 && line_error_mm>333)
				 {
					   followline.greitis_L=followline.greitis;
				     followline.greitis_R = (int16_t)(((int32_t)followline.greitis*(int32_t)(32769-followline.santikis))>>15);
				 }
				 else
				 {
					  followline.greitis_L=25;
					  followline.greitis_R = (int16_t)(((int32_t)followline.greitis_L*(int32_t)(32769-followline.santikis))>>15);
				 }

			}
			else if (followline.santikis<0)
			{
				//					166   333   500   666   833
				 if (line_error_mm<666 && line_error_mm>333)
				 {
					   followline.greitis_R=followline.greitis;
				     followline.greitis_L = (int16_t)(((int32_t)followline.greitis*(int32_t)(32769+(followline.santikis)))>>15);
				 }
				 else
				 {
					  //followline.greitis_R =(int16_t)((float)followline.greitis*0.8f);
					  followline.greitis_R=25;
					  followline.greitis_L = (int16_t)(((int32_t)followline.greitis_R*(int32_t)(32769+(followline.santikis)))>>15);
				 }
				 
			}
			else
			{
			   //lygu
				 followline.greitis_L=followline.greitis;
				 followline.greitis_R=followline.greitis;
			}
			
			set_motorR_speed(followline.greitis_L);
			set_motorL_speed(followline.greitis_R);
			//					166   333   500   666   833
			if ((followline.greitis_R<=1 && line_error_mm<166)
			     || (followline.greitis_R<=1 && line_error_mm>666)	
			)
			{
				 set_motorL_speed(-1);
			}
			
			//if (followline.greitis_L<2)
			//					166   333   500   666   833
			if ((followline.greitis_L<=1 && line_error_mm<166)
			     || (followline.greitis_L<=1 && line_error_mm>666)	
			)
			{
				set_motorR_speed(-1);
			}		


}

void follow_line_pwm(uint16_t line_error_mm, uint16_t line_flags)
{
//nieko nedarom
			//					166   333   500   666   833
			if (line_error_mm==1 || line_error_mm==1000)
			{
			   //pid_Reset_Integrator(&pid_follow);
				followline.santikis = pid_Controller(line_error_mm, 500, &pid_follow2);
			}
			else
			{
			   followline.santikis = pid_Controller(line_error_mm, 500, &pid_follow);
			}
					
			//data_visualizer_print(followline.santikis,line_error_mm,400);
			//mazinam greiti vienam varikiui kad suktusi i reikiama
			if (followline.santikis<0)
			{
				 //followline.greitis_L=followline.greitis/2;
				//					166   333   500   666   833
				 if (line_error_mm<666 && line_error_mm>333)
				 {
					   followline.greitis_L=followline.greitis;
				     followline.greitis_R = (int16_t)(((int32_t)followline.greitis*(int32_t)(32769+followline.santikis))>>15);
				 }
				 else
				 {
					  //followline.greitis_L=followline.greitis;
					  followline.greitis_L=followline.greitis_posukiai;
					  followline.greitis_R = (int16_t)(((int32_t)followline.greitis_L*(int32_t)(32769+followline.santikis))>>15);
				 }

			}
			else if (followline.santikis>0)
			{
				//followline.greitis_R=followline.greitis;
				//					166   333   500   666   833
				 if (line_error_mm<666 && line_error_mm>333)
				 {
					   followline.greitis_R=followline.greitis;
				     followline.greitis_L = (int16_t)(((int32_t)followline.greitis*(int32_t)(32769-(followline.santikis)))>>15);
				 }
				 else
				 {
					  followline.greitis_R=followline.greitis_posukiai;
					  followline.greitis_L = (int16_t)(((int32_t)followline.greitis_R*(int32_t)(32769-(followline.santikis)))>>15);
				 }
				 
			}
			else
			{
			   //lygu
				 followline.greitis_L=followline.greitis;
				 followline.greitis_R=followline.greitis;
			}
			
			
			
			
			//					166   333   500   666   833
			if ((followline.greitis_R<=1000 && line_error_mm<100)
			     || (followline.greitis_R<=1000 && line_error_mm>850)	
			)
			{
				set_motorR_speed_pwm(followline.stop_pwm);
			}
			else
			{
				 //if (followline.greitis_R<FOLLOW_LINE_MIN_PWM)
				 if (followline.greitis_R<followline.min_pwm)
				 {
				    //set_motorR_speed_pwm(FOLLOW_LINE_MIN_PWM);
					  set_motorR_speed_pwm(followline.min_pwm);
				 }
				 else
				 {
				    set_motorR_speed_pwm(followline.greitis_R);
				 }
			   
			}
			
			//					166   333   500   666   833
			if ((followline.greitis_L<=1000 && line_error_mm<100)
			     || (followline.greitis_L<=1000 && line_error_mm>850)	
			)
			{
				set_motorL_speed_pwm(followline.stop_pwm);
			}
      else
      {
				//if (followline.greitis_L<FOLLOW_LINE_MIN_PWM)
				if (followline.greitis_L<followline.min_pwm)
				{
				  // set_motorL_speed_pwm(FOLLOW_LINE_MIN_PWM);
					set_motorL_speed_pwm(followline.min_pwm);
				}
				else
				{
				   set_motorL_speed_pwm(followline.greitis_L);
				}
			  
			}				


}

//0 -pwm
//1 -pwm
//2 -pwm

//3 -enc
//4 -enc
//5 -enc

void follow_line_set(uint8_t nr)
{


}




void follow_line_1kHz(void)
{
  if (followline.delay_start)
	{
	  followline.delay_start--;
	}

}



//%20 pakleta itampa 11.5
void follow_line_set_pwm1(void)
{

   //pid_Init(70, 0, 90, &pid_follow);
	 //pid_Init(55, 0, 120, &pid_follow);
	 pid_Init(200, 0, 100, &pid_follow);
	 pid_Init(250, 0, 0, &pid_follow2);
	 
	 followline.state=FOLLOWLINE_START;
	 //followline.greitis = 13500;
	 //followline.greitis_posukiai = 12000;
	 //followline.stop_pwm = -6000;
	
	 followline.greitis = 16000;
	 followline.greitis_posukiai = 14000;
	 followline.stop_pwm = -10000;
	 followline.min_pwm=13000;
	
	 followline.pwm_start_flag = true;
	
	 followline.delay_start=1000;
	 GPIO_LED_RED_SENSOR_ON();

}

void follow_line_set_pwm2(void)
{
   //pid_Init(70, 0, 90, &pid_follow);
   //pid_Init(200, 0, 120, &pid_follow);
	 //pid_Init(350, 0, 0, &pid_follow2);
	 
	 followline.state=FOLLOWLINE_START;
	 //followline.greitis = 15000;
	 //followline.greitis_posukiai = 12500;
	 //followline.stop_pwm = -6000;
	
	
	 pid_Init(350, 0, 1000, &pid_follow);
	 pid_Init(350, 0, 0, &pid_follow2);
	 followline.greitis = 20000;
	 followline.greitis_posukiai = 17000;
	 followline.stop_pwm = -12000;
	 followline.min_pwm=16000;
	
	 followline.pwm_start_flag = true;
	
	 followline.delay_start=1000;
	 GPIO_LED_RED_SENSOR_ON();

}

void follow_line_set_pwm3(void)
{

   //pid_Init(65, 0, 95, &pid_follow);
	 //pid_Init(150, 0, 0, &pid_follow2);
	 
	 followline.state=FOLLOWLINE_START;
	 //followline.greitis = 20000;
	 //followline.greitis_posukiai = 13000;
	 //followline.stop_pwm = -9000;
	 
	 pid_Init(350, 0, 1000, &pid_follow);
	 pid_Init(350, 0, 0, &pid_follow2);
	 followline.greitis = 24000;
	 followline.greitis_posukiai = 17000;
	 followline.stop_pwm = -12000;
	 followline.min_pwm=18000;
	 followline.pwm_start_flag = true;
	
	 followline.delay_start=1000;
	 GPIO_LED_RED_SENSOR_ON();

}

void follow_line_set_pwm4(void)
{

   //pid_Init(65, 0, 100, &pid_follow);
	 //pid_Init(150, 0, 0, &pid_follow2);
	 
	 followline.state=FOLLOWLINE_START;
	 //followline.greitis = 25000;
	 //followline.greitis_posukiai = 14000;
	 //followline.stop_pwm = -9000;
	 pid_Init(350, 0, 1000, &pid_follow);
	 pid_Init(350, 0, 0, &pid_follow2);
	 followline.greitis = 27000;
	 followline.greitis_posukiai = 16000;
	 followline.stop_pwm = -12000;
	 followline.min_pwm=19000;
	 followline.pwm_start_flag = true;
	
	 followline.delay_start=1000;
	 GPIO_LED_RED_SENSOR_ON();

}

void follow_line_set_pwm5(void)
{

   //pid_Init(60, 0, 105, &pid_follow);
	 //pid_Init(175, 0, 0, &pid_follow2);
	 
	 followline.state=FOLLOWLINE_START;
	 //followline.greitis = 28000;
	 //followline.greitis_posukiai = 14000;
	 //followline.stop_pwm = -9500;
	 pid_Init(95, 0, 110, &pid_follow);
	 pid_Init(350, 0, 0, &pid_follow2);
	 followline.greitis = 29000;
	 //followline.greitis_posukiai = 16000;
	 followline.greitis_posukiai = 17000;
	 followline.stop_pwm = -12000;
	 //followline.min_pwm=19500;
	 followline.pwm_start_flag = true;
	 followline.min_pwm=10000;
	
	 followline.delay_start=1000;
	 GPIO_LED_RED_SENSOR_ON();
	//perfect

}

void follow_line_set_pwm6(void)
{

   //pid_Init(60, 0, 110, &pid_follow);
	 //pid_Init(200, 0, 0, &pid_follow2);
	 
	 followline.state=FOLLOWLINE_START;
	 //followline.greitis = 32760;
	 //followline.greitis_posukiai = 14500;
	 //followline.stop_pwm = -9500;
	
	 pid_Init(90, 0, 105, &pid_follow);
	 pid_Init(350, 0, 0, &pid_follow2);
	 followline.greitis = 31000;
	 followline.greitis_posukiai = 17000;
	 followline.stop_pwm = -13000;
	 //followline.min_pwm=19500;
	 followline.pwm_start_flag = true;
	 followline.min_pwm=13000;
	
	 followline.delay_start=1000;
	 GPIO_LED_RED_SENSOR_ON();

}



void follow_line_set_pwm7(void)
{


	 followline.state=FOLLOWLINE_START;
	 pid_Init(350, 0, 1000, &pid_follow);
	 pid_Init(350, 0, 0, &pid_follow2);
	 followline.greitis = 29000;
	 followline.greitis_posukiai = 16000;
	 followline.stop_pwm = -12000;
	 followline.min_pwm=19000;
	
	 followline.pwm_start_flag = true;
	
	 followline.delay_start=1000;
	 GPIO_LED_RED_SENSOR_ON();

}

void follow_line_set_pwm8(void)
{

	 
	 followline.state=FOLLOWLINE_START;
	
	 pid_Init(95, 0, 110, &pid_follow);
	 pid_Init(350, 0, 0, &pid_follow2);
	 followline.greitis = 29800;
	 followline.greitis_posukiai = 17000;
	 followline.stop_pwm = -12000;
	 followline.pwm_start_flag = true;
	 followline.min_pwm=20000;
	
	 followline.delay_start=1000;
	 GPIO_LED_RED_SENSOR_ON();
	//perfect

}

void follow_line_set_pwm9(void)
{


	 
	 followline.state=FOLLOWLINE_START;
	
	 //pid_Init(90, 0, 105, &pid_follow);
	 pid_Init(75, 0, 95, &pid_follow);
	 pid_Init(350, 0, 0, &pid_follow2);
	 followline.greitis = 32760;
	 followline.greitis_posukiai = 17000;
	 followline.stop_pwm = -13000;
	 followline.pwm_start_flag = true;
	
	 //followline.min_pwm=22000;
	 followline.min_pwm=24000;
	
	 followline.delay_start=1000;
	 GPIO_LED_RED_SENSOR_ON();

}


void follow_line_set_pwm10(void)
{


	 followline.state=FOLLOWLINE_START;
	 pid_Init(95, 0, 100, &pid_follow);
	 pid_Init(350, 0, 0, &pid_follow2);
	 followline.greitis = 29000;
	 followline.greitis_posukiai = 18000;
	 followline.stop_pwm = -12000;
	 followline.min_pwm=19000;
	
	 followline.pwm_start_flag = true;
	
	 followline.delay_start=1000;
	 GPIO_LED_RED_SENSOR_ON();

}

void follow_line_set_pwm11(void)
{

	 
	 followline.state=FOLLOWLINE_START;
	
	 pid_Init(95, 0, 110, &pid_follow);
	 pid_Init(350, 0, 0, &pid_follow2);
	 followline.greitis = 29800;
	 followline.greitis_posukiai = 18000;
	 followline.stop_pwm = -12000;
	 followline.pwm_start_flag = true;
	 followline.min_pwm=20000;
	
	 followline.delay_start=1000;
	 GPIO_LED_RED_SENSOR_ON();
	//perfect

}

void follow_line_set_pwm12(void)
{


	 
	 followline.state=FOLLOWLINE_START;
	
	 //pid_Init(90, 0, 105, &pid_follow);
	 pid_Init(75, 0, 95, &pid_follow);
	 pid_Init(350, 0, 0, &pid_follow2);
	 followline.greitis = 32760;
	 followline.greitis_posukiai = 18000;
	 followline.stop_pwm = -13000;
	 followline.pwm_start_flag = true;
	
	 //followline.min_pwm=22000;
	 followline.min_pwm=24000;
	
	 followline.delay_start=1000;
	 GPIO_LED_RED_SENSOR_ON();

}


void follow_line_set_pwm13(void)
{


	 followline.state=FOLLOWLINE_START;
	 pid_Init(95, 0, 100, &pid_follow);
	 pid_Init(350, 0, 0, &pid_follow2);
	 followline.greitis = 29000;
	 followline.greitis_posukiai = 20000;
	 followline.stop_pwm = -12000;
	 followline.min_pwm=19000;
	
	 followline.pwm_start_flag = true;
	
	 followline.delay_start=1000;
	 GPIO_LED_RED_SENSOR_ON();

}

void follow_line_set_pwm14(void)
{

	 
	 followline.state=FOLLOWLINE_START;
	
	 pid_Init(95, 0, 110, &pid_follow);
	 pid_Init(350, 0, 0, &pid_follow2);
	 followline.greitis = 29800;
	 followline.greitis_posukiai = 20000;
	 followline.stop_pwm = -12000;
	 followline.pwm_start_flag = true;
	 followline.min_pwm=20000;
	
	 followline.delay_start=1000;
	 GPIO_LED_RED_SENSOR_ON();
	//perfect

}

void follow_line_set_pwm15(void)
{


	 
	 followline.state=FOLLOWLINE_START;
	
	 //pid_Init(90, 0, 105, &pid_follow);
	 pid_Init(75, 0, 95, &pid_follow);
	 pid_Init(350, 0, 0, &pid_follow2);
	 followline.greitis = 32760;
	 followline.greitis_posukiai = 20000;
	 followline.stop_pwm = -13000;
	 followline.pwm_start_flag = true;
	
	 //followline.min_pwm=22000;
	 followline.min_pwm=24000;
	
	 followline.delay_start=1000;
	 GPIO_LED_RED_SENSOR_ON();

}



void follow_line_set_pwm16(void)
{


	 followline.state=FOLLOWLINE_START;
	 pid_Init(95, 0, 100, &pid_follow);
	 pid_Init(350, 0, 0, &pid_follow2);
	 followline.greitis = 29000;
	 followline.greitis_posukiai = 22000;
	 followline.stop_pwm = -12000;
	 followline.min_pwm=19000;
	
	 followline.pwm_start_flag = true;
	
	 followline.delay_start=1000;
	 GPIO_LED_RED_SENSOR_ON();

}

void follow_line_set_pwm17(void)
{

	 
	 followline.state=FOLLOWLINE_START;
	
	 pid_Init(95, 0, 110, &pid_follow);
	 pid_Init(350, 0, 0, &pid_follow2);
	 followline.greitis = 29800;
	 followline.greitis_posukiai = 22000;
	 followline.stop_pwm = -12000;
	 followline.pwm_start_flag = true;
	 followline.min_pwm=20000;
	
	 followline.delay_start=1000;
	 GPIO_LED_RED_SENSOR_ON();
	//perfect

}

void follow_line_set_pwm18(void)
{


	 
	 followline.state=FOLLOWLINE_START;
	
	 //pid_Init(90, 0, 105, &pid_follow);
	 pid_Init(75, 0, 95, &pid_follow);
	 pid_Init(350, 0, 0, &pid_follow2);
	 followline.greitis = 32760;
	 followline.greitis_posukiai = 22000;
	 followline.stop_pwm = -13000;
	 followline.pwm_start_flag = true;
	
	 //followline.min_pwm=22000;
	 followline.min_pwm=24000;
	
	 followline.delay_start=1000;
	 GPIO_LED_RED_SENSOR_ON();

}

void follow_line_set_pwm19(void)
{


	 followline.state=FOLLOWLINE_START;
	 pid_Init(95, 0, 100, &pid_follow);
	 pid_Init(350, 0, 0, &pid_follow2);
	 followline.greitis = 29000;
	 followline.greitis_posukiai = 25000;
	 followline.stop_pwm = -12000;
	 followline.min_pwm=19000;
	
	 followline.pwm_start_flag = true;
	
	 followline.delay_start=1000;
	 GPIO_LED_RED_SENSOR_ON();

}

void follow_line_set_pwm20(void)
{

	 
	 followline.state=FOLLOWLINE_START;
	
	 pid_Init(95, 0, 110, &pid_follow);
	 pid_Init(350, 0, 0, &pid_follow2);
	 followline.greitis = 29800;
	 followline.greitis_posukiai = 25000;
	 followline.stop_pwm = -12000;
	 followline.pwm_start_flag = true;
	 followline.min_pwm=20000;
	
	 followline.delay_start=1000;
	 GPIO_LED_RED_SENSOR_ON();
	//perfect

}

void follow_line_set_pwm21(void)
{


	 
	 followline.state=FOLLOWLINE_START;
	
	 //pid_Init(90, 0, 105, &pid_follow);
	 pid_Init(75, 0, 95, &pid_follow);
	 pid_Init(350, 0, 0, &pid_follow2);
	 followline.greitis = 32760;
	 followline.greitis_posukiai = 25000;
	 followline.stop_pwm = -13000;
	 followline.pwm_start_flag = true;
	
	 //followline.min_pwm=22000;
	 followline.min_pwm=24000;
	
	 followline.delay_start=1000;
	 GPIO_LED_RED_SENSOR_ON();

}


void follow_line_set_pwm22(void)
{


	 followline.state=FOLLOWLINE_START;
	 pid_Init(95, 0, 100, &pid_follow);
	 pid_Init(350, 0, 0, &pid_follow2);
	 followline.greitis = 29000;
	 followline.greitis_posukiai = 28000;
	 followline.stop_pwm = -12000;
	 followline.min_pwm=19000;
	
	 followline.pwm_start_flag = true;
	
	 followline.delay_start=1000;
	 GPIO_LED_RED_SENSOR_ON();

}

void follow_line_set_pwm23(void)
{

	 
	 followline.state=FOLLOWLINE_START;
	
	 pid_Init(95, 0, 110, &pid_follow);
	 pid_Init(350, 0, 0, &pid_follow2);
	 followline.greitis = 29800;
	 followline.greitis_posukiai = 28000;
	 followline.stop_pwm = -12000;
	 followline.pwm_start_flag = true;
	 followline.min_pwm=20000;
	
	 followline.delay_start=1000;
	 GPIO_LED_RED_SENSOR_ON();
	//perfect

}

void follow_line_set_pwm24(void)
{


	 
	 followline.state=FOLLOWLINE_START;
	
	 //pid_Init(90, 0, 105, &pid_follow);
	 pid_Init(75, 0, 95, &pid_follow);
	 pid_Init(350, 0, 0, &pid_follow2);
	 followline.greitis = 32760;
	 followline.greitis_posukiai = 28000;
	 followline.stop_pwm = -13000;
	 followline.pwm_start_flag = true;
	
	 //followline.min_pwm=22000;
	 followline.min_pwm=24000;
	
	 followline.delay_start=1000;
	 GPIO_LED_RED_SENSOR_ON();

}


void folow_line_stop(void)
{
   //GPIO_LED_RED_SENSOR_OFF();
	followline.state=FOLLOWLINE_STOP;
	followline.delay_start=1000;

}

