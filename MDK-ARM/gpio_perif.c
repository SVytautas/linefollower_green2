#include "stm32f1xx_hal.h"
#include "uart1.h"
#include "encoder.h"
#include "gpio_perif.h"
#include "adc_perif.h"
#include "followline.h"
#include "motor_perif.h"


gpio_btn_t gpio_btn={
  .btn_count = {0,0,0},

};


void gpio_perif_init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __GPIOC_CLK_ENABLE();
  __GPIOD_CLK_ENABLE();
  __GPIOA_CLK_ENABLE();
  __GPIOB_CLK_ENABLE();

	
	//LEDai -------------------------------- PC13 PC14 PC15
	 /*Configure GPIO pins : PC13 PC14 PC15 */
  GPIO_InitStruct.Pin = GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
  /*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
	
	
	
	//ledai raudoni phototranz imtuvui
	
	/*Configure GPIO pin : PB13 */

	GPIO_InitStruct.Pin = GPIO_PIN_13; //keiciamas i pa3
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	//HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);

	
	
	/*Configure GPIO pins : PB1 PB5 */
	/*
  GPIO_InitStruct.Pin = GPIO_PIN_5 |GPIO_PIN_1;
	//GPIO_InitStruct.Pin = GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
	*/
	
	
	/*Configure GPIO pin : PA11 PA10 */
	/*
  GPIO_InitStruct.Pin = GPIO_PIN_11 | GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);
	*/
	//---------LEDAI OK
	
	//ledai raudoni phototranz imtuvui
	/*Configure GPIO pin : PA6 */
	//GPIO_InitStruct.Pin = GPIO_PIN_6; //keiciamas i pa3
	//GPIO_InitStruct.Pin = GPIO_PIN_3; //keiciamas i pa3
  //GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  //GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
  //HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	//HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
	//HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);
	
	/*Configure GPIO pin : PA6 */
	//GPIO_InitStruct.Pin = GPIO_PIN_6; //keiciamas i pa3
	/*
	GPIO_InitStruct.Pin = GPIO_PIN_13; //keiciamas i pa3
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	//HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
	*/
	
	//VAriklio valdiklis 1variklis
	
	/*Configure GPIO pins : PB2  */
	/*
  GPIO_InitStruct.Pin = GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	 HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET);
	 HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_RESET);
	 */
	 //2variklis
	 /*Configure GPIO pins : PC13 PC14 PC15 */
	 /*
  GPIO_InitStruct.Pin = GPIO_PIN_13|GPIO_PIN_14;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
	*/
	
	//-------------Variklio valdiklis----------------------------------
	
	
	
	//------------------------------------------------------------------
	//pipsis
	GPIO_InitStruct.Pin = GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET); //nereguoja pakeistas MCU
	
	
  
   //USB enablas
  /*Configure GPIO pin Output Level */
 /*Configure GPIO pins : PB2 PB9 */
 /*
  GPIO_InitStruct.Pin = GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET);
 */
  
 

  

  //mygtuku iejimai
	/*Configure GPIO pin : PB8 */
	/*Configure GPIO pin : PB9 */
  //GPIO_InitStruct.Pin = GPIO_PIN_8;
	GPIO_InitStruct.Pin = GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
  /*Configure GPIO pin : PB12 */
  GPIO_InitStruct.Pin = GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  /*Configure GPIO pin : PA12 */
  GPIO_InitStruct.Pin = GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}



void gpio_button_handler_100Hz(void)
{

	//pirmas mygtukas STM
	if (!GPIO_BTN_STM_READ())
	{
		if (gpio_btn.btn_count[GPIO_BTN_STM_PTR]<UINT32_MAX)
		{
		   gpio_btn.btn_count[GPIO_BTN_STM_PTR]++;
			//PRINTF(".%lu",gpio_btn.btn_count[GPIO_BTN_STM_PTR]);
		}
		
		if (gpio_btn.btn_count[GPIO_BTN_STM_PTR]==GPIO_BTN_LONG)
		{
		   //PRINTF("btn stm long");
			 gpio_btn_event_handler(GPIO_BTN_STM_LONG);
		}
		
		//if 
	   
		 
	}
	else
  {
     if (gpio_btn.btn_count[GPIO_BTN_STM_PTR]> GPIO_BTN_DEBOUNCE && 
			 gpio_btn.btn_count[GPIO_BTN_STM_PTR]< GPIO_BTN_LONG
		 )
		{
			//PRINTF("btn stm short");
			gpio_btn_event_handler(GPIO_BTN_STM_SHORT);
			gpio_btn.btn_count[GPIO_BTN_STM_PTR]=0;
		
		}
    else if (gpio_btn.btn_count[GPIO_BTN_STM_PTR]>=GPIO_BTN_LONG)
		{
		  gpio_btn.btn_count[GPIO_BTN_STM_PTR]=0;
		}
		else
		{
			if (gpio_btn.btn_count[GPIO_BTN_STM_PTR]>0)
			{
			  gpio_btn.btn_count[GPIO_BTN_STM_PTR]--;
			}
		  
		}			
	}
	
	
	//kairys mygtukas
	//pirmas mygtukas STM
	if (!GPIO_BTN_LEFT_READ())
	{
		if (gpio_btn.btn_count[GPIO_BTN_LEFT_PTR]<UINT32_MAX)
		{
		   gpio_btn.btn_count[GPIO_BTN_LEFT_PTR]++;
			//PRINTF(".%lu",gpio_btn.btn_count[GPIO_BTN_STM_PTR]);
		}
		
		if (gpio_btn.btn_count[GPIO_BTN_LEFT_PTR]==GPIO_BTN_LONG)
		{
		  // PRINTF("btn left long");
			gpio_btn_event_handler(GPIO_BTN_LEFT_LONG);
		}
		
		//if 
	   
		 
	}
	else
  {
     if (gpio_btn.btn_count[GPIO_BTN_LEFT_PTR]> GPIO_BTN_DEBOUNCE && 
			 gpio_btn.btn_count[GPIO_BTN_LEFT_PTR]< GPIO_BTN_LONG
		 )
		{
			//PRINTF("btn left short");
			gpio_btn_event_handler(GPIO_BTN_LEFT_SHORT);
			gpio_btn.btn_count[GPIO_BTN_LEFT_PTR]=0;
		
		}
    else if (gpio_btn.btn_count[GPIO_BTN_LEFT_PTR]>=GPIO_BTN_LONG)
		{
		  gpio_btn.btn_count[GPIO_BTN_LEFT_PTR]=0;
		}
		else
		{
			if (gpio_btn.btn_count[GPIO_BTN_LEFT_PTR]>0)
			{
			  gpio_btn.btn_count[GPIO_BTN_LEFT_PTR]--;
			}
		  
		}			
	}
	
	//desinys mygtukas
	if (!GPIO_BTN_RIGHT_READ())
	{
		if (gpio_btn.btn_count[GPIO_BTN_RIGHT_PTR]<UINT32_MAX)
		{
		   gpio_btn.btn_count[GPIO_BTN_RIGHT_PTR]++;
			//PRINTF(".%lu",gpio_btn.btn_count[GPIO_BTN_STM_PTR]);
		}
		
		if (gpio_btn.btn_count[GPIO_BTN_RIGHT_PTR]==GPIO_BTN_LONG)
		{
		   //PRINTF("btn right long");
			 gpio_btn_event_handler(GPIO_BTN_RIGHT_LONG);
		}
		
		//if 
	   
		 
	}
	else
  {
     if (gpio_btn.btn_count[GPIO_BTN_RIGHT_PTR]> GPIO_BTN_DEBOUNCE && 
			 gpio_btn.btn_count[GPIO_BTN_RIGHT_PTR]< GPIO_BTN_LONG
		 )
		{
			//PRINTF("btn right short");
			gpio_btn_event_handler(GPIO_BTN_RIGHT_SHORT);
			gpio_btn.btn_count[GPIO_BTN_RIGHT_PTR]=0;
		
		}
    else if (gpio_btn.btn_count[GPIO_BTN_RIGHT_PTR]>=GPIO_BTN_LONG)
		{
		  gpio_btn.btn_count[GPIO_BTN_RIGHT_PTR]=0;
		}
		else
		{
			if (gpio_btn.btn_count[GPIO_BTN_RIGHT_PTR]>0)
			{
			  gpio_btn.btn_count[GPIO_BTN_RIGHT_PTR]--;
			}
		  
		}			
	}
	
	


}

uint8_t second_speed=0;

void gpio_btn_event_handler(uint8_t btn_event)
{
		switch (btn_event)
		{
			case GPIO_BTN_STM_SHORT:
			PRINTF("btn stm short\r\n");
			if (second_speed==2)
			{
			   follow_line_set_pwm8();	
			}
			else if (second_speed==1)
			{
			   follow_line_set_pwm5();	
			}
			else 
			{
			   follow_line_set_pwm2();	
			}
      			
      //GPIO_LED_BLUE_TOGGLE();			
			break;
		
		  case GPIO_BTN_LEFT_SHORT:
				
			PRINTF("btn left short\r\n");
			if (second_speed==2)
			{
			   follow_line_set_pwm7();
			}
			else if (second_speed==1)
			{
			   follow_line_set_pwm4();	
			}
			else
			{
			   follow_line_set_pwm1();
			}
			//GPIO_LED_RED_TOGGLE();
			break;
			
			case GPIO_BTN_RIGHT_SHORT:
			PRINTF("btn right short\r\n");	
      //GPIO_LED_GREEN_TOGGLE();
      if (second_speed==2)
			{
			   follow_line_set_pwm9();
			}
			else if (second_speed==1)
			{
			   follow_line_set_pwm6();	
			}
			else
			{
			   follow_line_set_pwm3();
			}				
			break;
			
			case GPIO_BTN_STM_LONG:
			PRINTF("btn stm long\r\n");		
			//GPIO_LED_RED_SENSOR_TOGGLE();
			set_clean_wheels();
			break;
			
			case GPIO_BTN_LEFT_LONG:
			PRINTF("btn left long\r\n");
      adc_perif_set_calibrating();			
			break;
			
			case GPIO_BTN_RIGHT_LONG:
			PRINTF("btn right long\r\n");
      second_speed++;
			if (second_speed>=3)
			{
			  second_speed=0;
			}
			
      if (second_speed==2)
			{
			   GPIO_LED_BLUE_OFF();		
				 GPIO_LED_RED_ON();
				 GPIO_LED_GREEN_OFF();
			}
			else if (second_speed==1)
			{
			   GPIO_LED_BLUE_OFF();		
				 GPIO_LED_RED_ON();
				 GPIO_LED_GREEN_ON();
			}
			else
			{
			  GPIO_LED_BLUE_ON();		
			  GPIO_LED_RED_ON();
				GPIO_LED_GREEN_ON();
			}
		  break;
		
		  default:
				break;
		
		}


}


