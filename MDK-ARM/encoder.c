#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "uart1.h"
#include "stm32f1xx_hal.h"
#include "encoder.h"
#include "data_visualizer_print.h"

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;
void MX_TIM1_Init(void);
void MX_TIM2_Init(void);

void encoder_int(void)
{


MX_TIM2_Init();
MX_TIM1_Init();	




}


/* TIM1 init function */
void MX_TIM1_Init(void)
{

  TIM_Encoder_InitTypeDef sConfig;
  TIM_MasterConfigTypeDef sMasterConfig;

  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 0;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 0xFFFF;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  sConfig.EncoderMode = TIM_ENCODERMODE_TI12;
  sConfig.IC1Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC1Filter = 0;
  sConfig.IC2Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC2Filter = 0;
  HAL_TIM_Encoder_Init(&htim1, &sConfig);

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig);
  HAL_TIM_Encoder_Start(&htim1, TIM_CHANNEL_1);
}

/* TIM2 init function */
void MX_TIM2_Init(void)
{

  TIM_Encoder_InitTypeDef sConfig;
  TIM_MasterConfigTypeDef sMasterConfig;

  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 0xFFFF;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim2.Init.RepetitionCounter = 0;
  sConfig.EncoderMode = TIM_ENCODERMODE_TI12;
  sConfig.IC1Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC1Filter = 0;
  sConfig.IC2Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC2Filter = 0;
  HAL_TIM_Encoder_Init(&htim2, &sConfig);

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig);
	HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_1);

}



int16_t encoder1(void)
{

   //return (uint32_t)__HAL_TIM_GET_COUNTER(&htim1);
	int16_t return_cnt=TIM1->CNT;;
	TIM1->CNT=0;
	return return_cnt;
	

}

int16_t encoder2(void)
{

   //return (uint32_t)__HAL_TIM_GET_COUNTER(&htim1);
	int16_t return_cnt=TIM2->CNT;;
	TIM2->CNT=0;
	return return_cnt;

}


void print_cnt(void)
{
	//PRINTF("1enc: %i\r\n", (int16_t)TIM1->CNT);
  //PRINTF("enc: %i\r\n", (int16_t)TIM2->CNT);
	PRINTF("1enc: %i\r\n", encoder1());
  PRINTF("enc: %i\r\n", encoder2());
	//data_visualizer_print(encoder1(),0,0);
	//data_visualizer_print(encoder2(),0,0);
	//TIM2->CNT=0;
}

