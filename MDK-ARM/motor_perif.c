
#include "stm32f1xx_hal.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>

#include "uart1.h"
#include "motor_perif.h"
#include "pid.h"
#include "encoder.h"
#include "data_visualizer_print.h"

//pid strukturosd r ir l varikliai
pidData_t pidR_motor;
pidData_t pidL_motor;



void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);
TIM_HandleTypeDef htim3;
static void MX_TIM3_Init(void);


motor_perif_t motor_perif={

	.update_speed_cnt=0,
	.motorR_speed = 100, //iki 70
	.motorL_speed = 100,
	.enable = false,
	.lock_to_much = false,
	.cnt_R_0 =0,
	.cnt_L_0 =0,
	.clean_wheals_delay=0,
};

void motor_perif_init(void)
{

  MX_TIM3_Init();
  //initinam pidus
	//250 50 20
	pid_Init(250, 50, 20, &pidL_motor);
	pid_Reset_Integrator(&pidL_motor);
	
	pid_Init(250, 50, 20, &pidR_motor);
	pid_Reset_Integrator(&pidR_motor);
	motor_perif.enable=true;
	set_motorR_speed(0);
	set_motorL_speed(0);

}


/* TIM3 init function */
void MX_TIM3_Init(void)
{

  TIM_MasterConfigTypeDef sMasterConfig;
  TIM_OC_InitTypeDef sConfigOC;

  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 1;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 0x0FFF;
	//htim3.Init.Period = 0x7FFF;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  HAL_TIM_PWM_Init(&htim3);

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig);

  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0x0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1);

  HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_2);

  HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_3);
  //sConfigOC.Pulse = 0x0;
  HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_4); //ledas
	


  HAL_TIM_MspPostInit(&htim3);
	
	//startinam kanalu pWM
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);
	
	//set_motorL_speed_pwm(12000);
	//set_motorR_speed_pwm(12000);
	 //set_motorL_speed(20);

}

//uint16_t cnt_test=0;
//bool fff;


void motor_perif_tick_1khz(void)
{

	if (!motor_perif.enable)
	{
	   return;
	}
	
	if (motor_perif.lock_to_much)
	{
		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 0);
		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 0);
		//kitas variklis
		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, 0);
		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, 0);
		return;
	
 	}
	
	motor_perif.update_speed_cnt++;
	if (motor_perif.update_speed_cnt>=MOTOR_SPEED_TIME_UPDATE)
	{
		   motor_perif.update_speed_cnt=0;
		   int16_t speed_R,speed_L;
		   int16_t outR_pwm,outL_pwm;
		   int16_t speedR_rev, speedL_rev;
	    //data_visualizer_print(encoder1(),0,0);
		  //PRINTF("enc: %i",(int32_t)encoder1);
		  //PRINTF("enc2: %i\r\n",(int32_t)encoder2);
		  //print_cnt();
		  if (motor_perif.motorR_speed<0)
			{
			     speed_R= -encoder1();
				  //data_visualizer_print(outR_pwm,motor_perif.motorR_speed,-encoder1());
			}
			else
			{
			     speed_R = encoder1();
			}
			
			if (speed_R==0)
			{
				
				if (motor_perif.cnt_R_0>MOTOR_PID_SPEED_CNT_RESET)
				{
					  motor_perif.cnt_R_0=0;
				    pid_Reset_Integrator(&pidR_motor);
				}
				else
				{
				    motor_perif.cnt_R_0++;
				}
			  
			}
			//speedR_rev = INT16_MAX/abs(speed_R);
			
			outR_pwm = pid_Controller(abs(motor_perif.motorR_speed), speed_R, &pidR_motor);
			//outR_pwm = pid_Controller(abs(motor_perif.motorR_speed), speedR_rev, &pidR_motor);
			//data_visualizer_print(outR_pwm,abs(motor_perif.motorR_speed),speed_R);
			//data_visualizer_print(outR_pwm,abs(motor_perif.motorR_speed),speedR_rev);
			//__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, 0xFFFF);
			
			if (outR_pwm>32000 && speed_R<10)
			{
			   //motor_perif.lock_to_much=true;
			}
			
			if (outR_pwm>0)
			{
				//__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, outR_pwm);
				 if (motor_perif.motorR_speed<0)
				 {
					 //__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, outR_pwm);
					 __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, outR_pwm>>3);
			     __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, 0);
				 
				 }
				 else if (motor_perif.motorR_speed>0)
				 {
						//MOTOR_R_FOR();
						__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, 0);
						//__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, outR_pwm);
					 __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, outR_pwm>>3);
					}
					else
					{
						__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, 0);
						__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, 0);
						//nulis stop
						//MOTOR_R_STOP();
						//abs(motor_speed_pwm)
						
				  }
					
			}
			else
			{
			  //__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, 0);
				__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, 0);
				__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, 0);
			}
			
			
			
			//L variklis
			if (motor_perif.motorL_speed<0)
			{
			     speed_L= -encoder2();
				  //data_visualizer_print(outR_pwm,motor_perif.motorR_speed,-encoder1());
			}
			else
			{
			     speed_L = encoder2();
			}
			
			if (speed_L==0)
			{
				
				if (motor_perif.cnt_L_0>MOTOR_PID_SPEED_CNT_RESET)
				{
					  motor_perif.cnt_L_0=0;
				    pid_Reset_Integrator(&pidL_motor);
				}
				else
				{
				    motor_perif.cnt_L_0++;
				}
			  
			}
			
			
			
			
			outL_pwm = pid_Controller(abs(motor_perif.motorL_speed), speed_L, &pidL_motor);
			//data_visualizer_print(outL_pwm,abs(motor_perif.motorL_speed),speed_L);
			
			if (outL_pwm>32000 && speed_L<10)
			{
			  //motor_perif.lock_to_much=true;
			}
			
			if (outL_pwm>0)
			{
				//__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, outR_pwm);
				 if (motor_perif.motorL_speed<0)
				 {
					 __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 0);
			     //__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, outL_pwm);
				   __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, outL_pwm>>3);
				 }
				 else if (motor_perif.motorL_speed>0)
				 {
						//MOTOR_R_FOR();
						//__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, outL_pwm);
					  __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, outL_pwm>>3);
			      __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 0);
					}
					else
					{
						__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 0);
			      __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 0);
						//nulis stop
						//MOTOR_R_STOP();
						//abs(motor_speed_pwm)
						
				  }
					
			}
			else
			{
			  //__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, 0);
				__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 0);
			  __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 0);
			}
			
			
			
		  
	
	}
	
	
	if (motor_perif.clean_wheals_delay)
	{
	   motor_perif.clean_wheals_delay--;
		 if (!motor_perif.clean_wheals_delay)
		 {
		    //stabdom
			  set_motorL_speed_pwm(0);
			  set_motorR_speed_pwm(0);
		    motor_perif.enable=false;
		 }
	   
	}
	
	/*
	cnt_test++;
	if (cnt_test>2000)
	{
	  //reversinam
		cnt_test=0;
		fff = !fff;
		if (fff)
		{
		   set_motorL_speed(motor_perif.motorL_speed);
		   set_motorR_speed(motor_perif.motorR_speed);
		}
		else
		{
		   set_motorL_speed(motor_perif.motorL_speed);
		   set_motorR_speed(motor_perif.motorR_speed);
		}
		
	}
	*/
	
	

}


void set_motorR_speed(int16_t motor_speed)
{
    motor_perif.motorR_speed = motor_speed;
	
	  if (motor_perif.motorR_speed!=motor_speed)
		{
		   //resetinam I
			//pid_Reset_Integrator(&pidR_motor);
			
			if (motor_perif.lock_to_much)
			{
			   //__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, 0);
			}
			else
			{
			   //__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, (motor_speed*450));
			}
		}
		
	  if (motor_speed<0)
		{
		  //MOTOR_R_REV();
			if (motor_speed<-MOTOR_MAX_SPEED)
			{
			  motor_perif.motorR_speed = -MOTOR_MAX_SPEED;
			}
		}
		else if (motor_speed>0)
		{
		  //MOTOR_R_FOR();
			if (motor_speed>MOTOR_MAX_SPEED)
			{
			  motor_perif.motorR_speed = MOTOR_MAX_SPEED;
			}
		}
		else
		{
			//nulis stop
			//motor_perif.motorR_speed = 0;
			//MOTOR_R_STOP();
		  
		}
		motor_perif.enable=true;

}


void set_motorL_speed(int16_t motor_speed)
{
	  
    motor_perif.motorL_speed = motor_speed;
	  if (motor_perif.motorL_speed!=motor_speed)
		{
		   //resetinam I
			//pid_Reset_Integrator(&pidL_motor);
			if (motor_perif.lock_to_much)
			{
			   //__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 0);
			}
			else
			{
			   //__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, (motor_speed*450));
			}
			
		}
	
	  if (motor_speed<0)
		{
		  //MOTOR_L_REV();
			if (motor_speed<-MOTOR_MAX_SPEED)
			{
			  motor_perif.motorL_speed = -MOTOR_MAX_SPEED;
			}
		}
		else if (motor_speed>0)
		{
		  //MOTOR_L_FOR();
			if (motor_speed>MOTOR_MAX_SPEED)
			{
			  motor_perif.motorL_speed = MOTOR_MAX_SPEED;
			}
		}
		else
		{
			//nulis stop
			//motor_perif.motorR_speed = 0;
			//MOTOR_L_STOP();
			//motor_perif.motorL_speed=motor_speed;
		  
		}
		motor_perif.enable=true;

}

void set_motorL_speed_pwm(int16_t motor_speed_pwm)
{

    motor_perif.enable=false;
	  if (motor_speed_pwm<0)
		{
		  //MOTOR_L_REV();
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 0);
			//__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, abs(motor_speed_pwm));
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, abs(motor_speed_pwm)>>3);
		}
		else if (motor_speed_pwm>0)
		{
		  //MOTOR_L_FOR();
			//__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, abs(motor_speed_pwm));
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, abs(motor_speed_pwm)>>3);
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 0);
		}
		else
		{
			//nulis stop
			//MOTOR_L_STOP();
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 0);
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 0);
		  
		}
		
		//__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, abs(motor_speed_pwm));
}

void set_motorR_speed_pwm(int16_t motor_speed_pwm)
{

    motor_perif.enable=false;
	  if (motor_speed_pwm<0)
		{
		  //MOTOR_R_REV();
			//__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, abs(motor_speed_pwm));
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, abs(motor_speed_pwm)>>3);
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, 0);
		}
		else if (motor_speed_pwm>0)
		{
		  //MOTOR_R_FOR();
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, 0);
			//__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, abs(motor_speed_pwm));
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, abs(motor_speed_pwm)>>3);
		}
		else
		{
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, 0);
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, 0);
			//nulis stop
			//MOTOR_R_STOP();
			//abs(motor_speed_pwm)
		  
		}
		
		//__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, abs(motor_speed_pwm));
}


void set_clean_wheels(void)
{

   motor_perif.clean_wheals_delay = 10000; //10s valome ratus
	 set_motorR_speed(30);
	 set_motorL_speed(30);
	 encoder1(); //clerinam encoderius
	 encoder2();

}


