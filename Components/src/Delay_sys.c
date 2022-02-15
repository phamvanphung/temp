#include <stdio.h>
#include "M051Series.h"
#include "Delay_sys.h"


volatile uint32_t Millis;

volatile uint32_t Micros;


void SysTick_Handler(void){

Millis++;
}


void Set_sys_init(void){
			if (SysTick_Config (SystemCoreClock / 1000)) //1ms per interrupt 
		{
					while (1);
		}
		NVIC_SetPriority(SysTick_IRQn,0);
		NVIC_EnableIRQ(SysTick_IRQn);
		Millis = 0;
}

uint32_t micros(void)
{

    Micros = Millis*1000+(SystemCoreClock/1000-SysTick->VAL)/72;
		return Micros;

}

uint32_t millis(void)
{
return Millis;
}


void delay_ms(uint32_t nTime)
{
uint32_t curTime = Millis;
while((nTime-(Millis-curTime)) > 0);
}

void delay_us(uint32_t nTime)
{
uint32_t curTime = Micros;
while((nTime-(Micros-curTime)) > 0);
}


