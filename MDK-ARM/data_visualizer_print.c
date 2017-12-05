#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "uart1.h"
#include "data_visualizer_print.h"


void data_visualizer_print(int16_t D1, int16_t D2, int16_t D3)
{
	/*/PID grafiko isvedimas atmel data visualizer
	usart_putchar(USART_bin_SERIAL, 0x03); //
	//int16-temperatura
	usart_putchar(USART_bin_SERIAL, temperatura_ismatuota&0x00FF);
	usart_putchar(USART_bin_SERIAL, temperatura_ismatuota>>8);
	//-------------------------------
	//--------galia is pid int16-----------------
	usart_putchar(USART_bin_SERIAL, temp.galia_on&0x00FF);
	usart_putchar(USART_bin_SERIAL, temp.galia_on>>8);
	//--------temp set-------------------
	usart_putchar(USART_bin_SERIAL, temp.temperatura_to&0x00FF);
	usart_putchar(USART_bin_SERIAL, temp.temperatura_to>>8);
	//--------ka nustatome su dac uint16-------------------
	usart_putchar(USART_bin_SERIAL, temp.galia_dac&0x00FF);
	usart_putchar(USART_bin_SERIAL, temp.galia_dac>>8);
	usart_putchar(USART_bin_SERIAL, 0xFC);
	//*/
	
	//ivedam starta
	uart1_put_char(0x03);
	//D1
	uart1_put_char((uint16_t)D1 & 0x00FF);
	uart1_put_char((uint16_t)D1 >>8);
	//D2
	uart1_put_char((uint16_t)D2 & 0x00FF);
	uart1_put_char((uint16_t)D2 >>8);
	//D3
	uart1_put_char((uint16_t)D3 & 0x00FF);
	uart1_put_char((uint16_t)D3 >>8);
	//pabaiga
	uart1_put_char(0xFC);
	

}