#include "adc_perif.h"
#include "stm32f1xx_hal.h"
#include "uart1.h"
#include "encoder.h"
#include "gpio_perif.h"
#include "beep.h"
#include "main.h"
#include "eeprom_ext.h"
#include "followline.h"

ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

__IO uint16_t ADC1ConvertedValues[12];
uint32_t suma[6];
uint16_t adc_res[6];
uint8_t suma_cnt=0;


adc_perif_t adc_perif = {
  .bat_voltage_adc = 0,
	.bat_voltage_suma = 0,
	.bat_voltage_cnt = 0,
	.bat_voltage = 10, //kad nepisetu kai dar nepasikite
	
	

};


//kelibravimo ,skakitimo handleris
adc_perif_sensor_t adc_perif_sensor ={
   .calib_state = ADC_PERIF_CALIB_NONE,
	 .calib_delay = 0,
	.calib_on = false,
	 

};


adc_perif_kalib_t adc_perif_kalib = {
   .min_adc = {0,0,0,0,0},
	 .max_adc = {UINT16_MAX, UINT16_MAX,UINT16_MAX,UINT16_MAX,UINT16_MAX},

};

static void MX_DMA_Init(void);
static void MX_ADC1_Init(void);

void adc_perif_init(void)
{
 MX_DMA_Init();
 MX_ADC1_Init();

}

void adc_perif_init2(void)
{
   //initinam klaibravimo matavimo duomenis
   for (uint8_t i = 0; i<ADC_PERIF_ADC_INS;i++)
	 {
			adc_perif_sensor.sensor_coef[i]=(((uint32_t)1)<<(ADC_PERIF_SENSOR_NORM+ADC_PERIF_SENSOR_NORM_SHIFT))/(adc_perif_kalib.max_adc[i]-adc_perif_kalib.min_adc[i]);
	 }
}



void MX_ADC1_Init(void)
{

  ADC_ChannelConfTypeDef sConfig;

    /**Common config 
    */
  hadc1.Instance = ADC1;
  hadc1.Init.ScanConvMode = ADC_SCAN_ENABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
	hadc1.Init.NbrOfDiscConversion   = 6;  
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	//hadc1.Init.ExternalTrigConv =ADC_INJECTED_SOFTWARE_START ;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 6;
  HAL_ADC_Init(&hadc1);

    /**Configure Regular Channel 
    */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = 1;
  //sConfig.SamplingTime = ADC_SAMPLETIME_55CYCLES_5;
	sConfig.SamplingTime = ADC_SAMPLING_TIME;
  HAL_ADC_ConfigChannel(&hadc1, &sConfig);

    /**Configure Regular Channel 
    */
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = 2;
  HAL_ADC_ConfigChannel(&hadc1, &sConfig);

    /**Configure Regular Channel 
    */
  sConfig.Channel = ADC_CHANNEL_2;
  sConfig.Rank = 3;
  HAL_ADC_ConfigChannel(&hadc1, &sConfig);

    /**Configure Regular Channel 
    */
  sConfig.Channel = ADC_CHANNEL_3;
  sConfig.Rank = 4;
  HAL_ADC_ConfigChannel(&hadc1, &sConfig);
	
	//sConfig.Channel = ADC_CHANNEL_6;
  //sConfig.Rank = 4;
  //HAL_ADC_ConfigChannel(&hadc1, &sConfig);


    /**Configure Regular Channel 
    */
  sConfig.Channel = ADC_CHANNEL_4;
  sConfig.Rank = 5;
  HAL_ADC_ConfigChannel(&hadc1, &sConfig);

    /**Configure Regular Channel 
    */
  sConfig.Channel = ADC_CHANNEL_5;
  sConfig.Rank = 6;
  HAL_ADC_ConfigChannel(&hadc1, &sConfig);
	
	//Paleidziam dma handleri
	HAL_ADC_Start_DMA(&hadc1,
                        (uint32_t *)ADC1ConvertedValues,
                        6
                       );
											 
	HAL_ADC_Start_IT(&hadc1); //paleidziam Adc
	//HAL_ADC_PollForConversion(&hadc1, 1);

}


void MX_DMA_Init(void) 
{
  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);

}

uint8_t ok_sec=0;
uint8_t con_ok=0;
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *AdcHandle)
{
  /* Report to main program that ADC sequencer has reached its end */
	
	//HAL_ADC_Stop(&hadc1); //paleidziam Adc
	//HAL_ADC_Start_DMA(
  //PRINTF("!\r\n");
	//PRINTF("%u\r\n",ADC1ConvertedValues[0]);
	//PRINTF("%u\r\n",ADC1ConvertedValues[1]);
	//PRINTF("%u\r\n",ADC1ConvertedValues[2]);
	//PRINTF("%u\r\n",ADC1ConvertedValues[3]);
	//PRINTF("%u\r\n",ADC1ConvertedValues[4]);
	//PRINTF("%u\r\n",ADC1ConvertedValues[5]);
	//HAL_ADC_Start_DMA(&hadc1,
   //                     (uint32_t *)ADC1ConvertedValues,
   //                     6
   //                    );
	
	for (uint8_t i=0;i<5;i++)
	{
	   suma[i]+=ADC1ConvertedValues[i];
	}
	
	
	
	suma_cnt++;
	if(suma_cnt>=ADC_CNT_SIZE)
	{
		  suma_cnt=0;
	   for (uint8_t i=0;i<ADC_PERIF_ADC_INS;i++)
	   {
	     adc_res[i]=(uint16_t)(suma[i]/ADC_CNT_SIZE);
			 suma[i]=0;
			 
			 if (adc_res[i]>adc_perif_kalib.min_adc[i])
			 {
				 //minusuojam ir normalizuojam
				 adc_perif_sensor.sensor_linear[i]=adc_res[i]-adc_perif_kalib.min_adc[i];
				 adc_perif_sensor.sensor_linear[i]=(uint16_t)(((uint32_t)adc_perif_sensor.sensor_linear[i]*adc_perif_sensor.sensor_coef[i])>>ADC_PERIF_SENSOR_NORM_SHIFT);
				 if (adc_perif_sensor.sensor_linear[i]>4095)
				 {
				    adc_perif_sensor.sensor_linear[i]=4095;
				 }
				 
			 
			 }
			 else
			 {
			   adc_perif_sensor.sensor_linear[i]=0;
			   
			 }
	   }
		 
		 //apverciam matavima
		 for (uint8_t i=0;i<ADC_PERIF_ADC_INS;i++)
	   {
			 adc_perif_sensor.sensor_linear_rev[i]=4096-adc_perif_sensor.sensor_linear[i];
			 
		 }
		 adc_perif_calc_error();
		 
	}
	
	//baterijo itampa
	
	adc_perif.bat_voltage_suma+=ADC1ConvertedValues[5];
	adc_perif.bat_voltage_cnt++;
	if(adc_perif.bat_voltage_cnt>=ADC_PERIF_SAMPLES_BAT_CNT)
	{
	  adc_perif.bat_voltage_adc = (uint16_t)(adc_perif.bat_voltage_suma/ADC_PERIF_SAMPLES_BAT_CNT);
		ok_sec=1; //uzdedam flag kad suskaiciavom
		
		//nulinam  pointerius ir suma
		adc_perif.bat_voltage_suma=0;
		adc_perif.bat_voltage_cnt=0;
		//suskaiciuojam baterijo itampa
		adc_perif.bat_voltage = adc_perif.bat_voltage_adc*ADC_PERIF_1ADC_mV;
		//PRINTF("Bat itampa : %f ",adc_perif.bat_voltage);
		//jeigu baterija isikrovus setinam pipsejima
		
	}
	
	
	
	HAL_ADC_Start_IT(&hadc1);
	
	
	
	//HAL_ADC_Start_DMA(&hadc1,
  //                      (uint32_t *)ADC1ConvertedValues,
  //                     6
  //                     );
	//HAL_ADC_Start_IT(&hadc1); //paleidziam Adc
	 // con_ok=1;
	
	
	//HAL_ADC_Start_IT(&hadc1); //paleidziam Adc
	//HAL_ADC_Start(&hadc1); //paleidziam Adc
	//HAL_ADC_PollForConversion(&hadc1, 1);
}




void adc_perif_1kHz(void)
{
	
	//baterijos check handleris jeigu baterija isikrovus setinam pipset 2s
	 if (adc_perif.bat_voltage<ADC_BATERIJA_MIN_VOLTAGE)
	 {
	    //setinam piset 2s
		  beep_set_kartai(ADC_BAT_BEEP_TIME, ADC_BAT_BEEP_CNT);
	 }
	 
	 
	 if (adc_perif_sensor.calib_delay)
	 {
	   adc_perif_sensor.calib_delay--;
	 }
	 
}



void adc_calib_sensor_handler(void)
{
   switch (adc_perif_sensor.calib_state)
	 {
			 case ADC_PERIF_CALIB_NONE:
			 //nieko nedarome
		   
			 break;
			 
			 case ADC_PERIF_CALIB_INIT:
			 //initiname kalibravima
		   //nuresetinam kalibravimo struktura
			 //memset(&adc_perif_kalib.min_adc,UINT16_MAX,sizeof(adc_perif_kalib.min_adc));
			 //memset(&adc_perif_kalib.max_adc,0,sizeof(adc_perif_kalib.max_adc));
			 
			 
			 
			 //ijungiame raudonus led adc
			 GPIO_LED_RED_SENSOR_ON();
			 
			 if (!adc_perif_sensor.calib_delay)
			 {
						 for (uint8_t i = 0; i<ADC_PERIF_ADC_INS;i++)
					 {
						 adc_perif_kalib.min_adc[i]=UINT16_MAX;
						 adc_perif_kalib.max_adc[i]=0;
						 
					 }
			    adc_perif_sensor.calib_state= ADC_PERIF_CALIBRATING;
			    adc_perif_sensor.calib_delay = 15000; //10s kalibruojam
					GPIO_LED_RED_OFF();
					GPIO_LED_BLUE_OFF();
					GPIO_LED_GREEN_OFF();
			 }
			 
			 
			 
			 
			 break;
			 
			 case ADC_PERIF_CALIBRATING:
			 //nieko nedarome
			 
			 
			 //praeiname visus kanalus
			 for (uint8_t i = 0; i<ADC_PERIF_ADC_INS;i++)
			 {
			    if (adc_res[i]<adc_perif_kalib.min_adc[i])
					{
					   adc_perif_kalib.min_adc[i] = adc_res[i];
					}
			    
					if (adc_res[i]>adc_perif_kalib.max_adc[i])
					{
					   adc_perif_kalib.max_adc[i] = adc_res[i];
					}
			 }
			 
			 
			 
			 if (!adc_perif_sensor.calib_delay)
			 {
			   adc_perif_sensor.calib_state= ADC_PERIF_CALIB_END;
			 }
			 break;
			 
			 case ADC_PERIF_CALIB_END:
			 //nieko nedarome
			 //isjungiame led
		   GPIO_LED_RED_SENSOR_OFF();
			 PRINTF("ADC   1   2   3   4   5\r\n");
			 PRINTF("MAX   %u   %u   %u   %u   %u\r\n", adc_perif_kalib.max_adc[0],
			  adc_perif_kalib.max_adc[1],
			  adc_perif_kalib.max_adc[2],
			  adc_perif_kalib.max_adc[3],
			  adc_perif_kalib.max_adc[4]);
			 PRINTF("MIN   %u   %u   %u   %u   %u\r\n",adc_perif_kalib.min_adc[0],
			 adc_perif_kalib.min_adc[1],
			 adc_perif_kalib.min_adc[2],
			 adc_perif_kalib.min_adc[3],
			 adc_perif_kalib.min_adc[4]);
			 adc_perif_sensor.calib_state=  ADC_PERIF_CALIB_NONE;
			 for (uint8_t i = 0; i<ADC_PERIF_ADC_INS;i++)
			 {
			 adc_perif_sensor.sensor_coef[i]=(((uint32_t)1)<<(ADC_PERIF_SENSOR_NORM+ADC_PERIF_SENSOR_NORM_SHIFT))/(adc_perif_kalib.max_adc[i]-adc_perif_kalib.min_adc[i]);
			 }
			 
			 //isaugom i eeprom duomenis
			 eeprom_ext_write_block((char *)&adc_perif_kalib,MAIN_EE_ADRESS_ADC_PREIF_KALIB,sizeof(adc_perif_kalib));
			 
			 break;
			 
			 
			 default:
				adc_perif_sensor.calib_state=  ADC_PERIF_CALIB_NONE;
			 
			 break;
			 
	   
	 
	 }



}


void adc_perif_set_calibrating(void)
{

   adc_perif_sensor.calib_state=  ADC_PERIF_CALIB_INIT;
	 adc_perif_sensor.calib_delay = 500;
}


void adc_perif_print_test(void)
{

    PRINTF("MIN   %u   %u   %u   %u   %u\r\n",adc_perif_sensor.sensor_linear[0],
			 adc_perif_sensor.sensor_linear[1],
			 adc_perif_sensor.sensor_linear[2],
			 adc_perif_sensor.sensor_linear[3],
			 adc_perif_sensor.sensor_linear[4]);

}

void adc_perif_print_test_rev(void)
{

    PRINTF("REV   %u   %u   %u   %u   %u\r\n",adc_perif_sensor.sensor_linear_rev[0],
			 adc_perif_sensor.sensor_linear_rev[1],
			 adc_perif_sensor.sensor_linear_rev[2],
			 adc_perif_sensor.sensor_linear_rev[3],
			 adc_perif_sensor.sensor_linear_rev[4]);

}


void adc_perif_kalib_print(void)
{
       PRINTF("ADC   1   2   3   4   5\r\n");
			 PRINTF("MAX   %u   %u   %u   %u   %u\r\n", adc_perif_kalib.max_adc[0],
			  adc_perif_kalib.max_adc[1],
			  adc_perif_kalib.max_adc[2],
			  adc_perif_kalib.max_adc[3],
			  adc_perif_kalib.max_adc[4]);
			 PRINTF("MIN   %u   %u   %u   %u   %u\r\n",adc_perif_kalib.min_adc[0],
			 adc_perif_kalib.min_adc[1],
			 adc_perif_kalib.min_adc[2],
			 adc_perif_kalib.min_adc[3],
			 adc_perif_kalib.min_adc[4]);
}


void adc_perif_calc_error(void)
{

   //Weigted average 0-800;
//sensor 		0 		1 		2 		3 		4 		
//					100		250		400		550		700		
//					166   333   500   666   833     
	  //
	  //uint16_t line_pos_error;
	  
	  uint32_t skait = 166 * adc_perif_sensor.sensor_linear_rev[0]+
	  333 * adc_perif_sensor.sensor_linear_rev[1]+
	  500 * adc_perif_sensor.sensor_linear_rev[2]+
	  666 * adc_perif_sensor.sensor_linear_rev[3]+
	  833 * adc_perif_sensor.sensor_linear_rev[4];
	
	  uint32_t dalik = adc_perif_sensor.sensor_linear_rev[0]+
	  adc_perif_sensor.sensor_linear_rev[1]+
	  adc_perif_sensor.sensor_linear_rev[2]+
	  adc_perif_sensor.sensor_linear_rev[3]+
	  adc_perif_sensor.sensor_linear_rev[4];
	  
	  //line_pos_error = (uint16_t)(skait/dalik);
		adc_perif_sensor.sensor_err = (uint16_t)(skait/dalik);
		
		//PRINTF("line err: %u \r\n",adc_perif_sensor.sensor_err);
	
	  //parasukam visus jutiklius ir sudedam bitus
		for (uint8_t i = 0; i<ADC_PERIF_ADC_INS;i++)
		{
			    if (adc_perif_sensor.sensor_linear_rev[i]>ADC_PERIF_SENSOR_BIT_SET)
					{
						//uzdeddam jutiklio flag
					   SETBIT(adc_perif_sensor.sensor_flag,i);
					}
					else if (adc_perif_sensor.sensor_linear_rev[i]<ADC_PERIF_SENSOR_BIT_CLEAR)
					{
						//nuimam jutiklio flag
					   CLEARBIT(adc_perif_sensor.sensor_flag,i);
					
					}
					else
					{
					  //nieko nedarom histerize
					}
		}
		
		
		//PRINTF("sensor: ");
		
		//for (uint8_t i = 0; i<ADC_PERIF_ADC_INS;i++)
		//{
		//	    if (CHECKBIT(adc_perif_sensor.sensor_flag,i))
		//			{
					 // PRINTF("1 ");
		//			}
		//			else
		//			{ 
					 // PRINTF("0 ");
		//			}
		//}
		
		if (adc_perif_sensor.sensor_flag)
		{
		    
			  if (adc_perif_sensor.sensor_flag_0_cnt)
				{
					
					 //adc_perif_sensor.sensor_flag_0_cnt--;
					 adc_perif_sensor.sensor_flag_0_cnt=0;
				  // if (adc_perif_sensor.sensor_err_last>450)
					// {
					//		adc_perif_sensor.sensor_err = 800;
					// }
					//	else if (adc_perif_sensor.sensor_err_last<450)
					//	{
					//		adc_perif_sensor.sensor_err = 1;
					//	}
					//	else
						//{
						   //adc_perif_sensor.sensor_err_last = adc_perif_sensor.sensor_err;
						//}
				}
				else
				{
				   adc_perif_sensor.sensor_err_last = adc_perif_sensor.sensor_err;
					 adc_perif_sensor.sensor_flag_last = adc_perif_sensor.sensor_flag;
				}
		}
		else
		{
			 if(adc_perif_sensor.sensor_flag_0_cnt<ADC_PERIF_SENSOR_DEBOUNCE)
			 {
			    adc_perif_sensor.sensor_flag_0_cnt++;
			 
			 }
			 else
			 {   //					166   333   500   666   833
			     if (adc_perif_sensor.sensor_err_last>=500)
					 {
							adc_perif_sensor.sensor_err = 1000;
						  if (CHECKBIT(adc_perif_sensor.sensor_flag_last,0))
							{
							   adc_perif_sensor.sensor_err = 1;
							}
						 
					 }
						else if (adc_perif_sensor.sensor_err_last<500)
						{
							adc_perif_sensor.sensor_err = 1;
							if (CHECKBIT(adc_perif_sensor.sensor_flag_last,5))
							{
							   adc_perif_sensor.sensor_err = 1000;
							}
						}
						else
						{
						   adc_perif_sensor.sensor_err_last = adc_perif_sensor.sensor_err;
							 
						}
						
						
						//if (adc_perif_sensor.sensor_err_last==0x30)
						//{
						//   adc_perif_sensor.sensor_err = 1000;
						//}
						//else if (adc_perif_sensor.sensor_err_last==0x15)
						//{
						//   adc_perif_sensor.sensor_err = 1;
						//}
						
						
			 }
			 				
		   //adc_perif_sensor.sensor_err = adc_perif_sensor.sensor_err_last;
		}
		
		followline_handler(adc_perif_sensor.sensor_err,adc_perif_sensor.sensor_flag);



}




