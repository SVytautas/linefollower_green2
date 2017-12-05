#include "adc_perif.h"
#include "stm32f1xx_hal.h"
#include "uart1.h"
#include "encoder.h"
#include "gpio_perif.h"
#include "beep.h"

beep_t beep = {
  .enable = false,
	.beep_time_ms=0,
	.beep_kartai = 0,
	.beep_time_cnt=0,
	.beep_kartai_cnt=0,
};

void beep_handler_1kHz(void)
{
	
	//beeb handleris
	if (beep.enable)
	{
		if (beep.beep_kartai_cnt) //jeigu yra ka pipset
		{
			if (beep.beep_time_cnt>0) //jeigu nepraejo laikas toglinam
			{
				 //jeigu dar yra pipsejimoa
				 beep.beep_time_cnt--;
			   if (beep.beep_time_cnt>0)
				 {
				   BEEP_TOGGLE;
				 }
				 else
				 {
				   BEEP_OFF;
					 //pereinam prie beep off laiko
					 beep.beep_time_cnt= -beep.beep_time_ms;
				 }
			
			}
			else if (beep.beep_time_cnt<0)
			{
			   beep.beep_time_cnt++;
				 if (beep.beep_time_cnt==0) 
				 {
				     BEEP_OFF;
					   //minusuojam kartus
					  beep.beep_kartai_cnt--;
					 //sudedam nauja beep jei reikes
					 beep.beep_time_cnt = beep.beep_time_ms;
				 }
			  
			
			
			}
			
		
		}
		else
		{
		  beep.enable=false;
		}
	}
	  
}


//jeigu grazinamas true beep neuzimtas
bool beep_set_kartai(uint16_t laikas_ms, uint16_t kartai)
{
    if (!beep.enable)
		{
		   beep.beep_time_ms = laikas_ms;
			 beep.beep_time_cnt = laikas_ms; //pradenam nuo pipsejimo
			 beep.beep_kartai_cnt = kartai;
			 beep.beep_kartai = kartai;
			 beep.enable = true;
			
			return true;
		}
		else
		{
		  return false;
		}

}


