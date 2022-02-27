#include <stdio.h>
#include "M051Series.h"
#include "gpio_adc_init.h"
#include "sub_define.h"

extern uint16_t Xung_CB_LL;
extern uint8_t tansoPWM;
void GPIO_Init(void)
{
	// init gpio input
	GPIO_SetMode(P4,BIT3,GPIO_PMD_INPUT);      // Auto van 3
	GPIO_SetMode(P3,BIT5,GPIO_PMD_INPUT);      // Phao bon nuoc
	//AutoVan3 = off;
	//PhaoBonNuoc = off;
	// init gpio output
	GPIO_SetMode(P4,BIT2,GPIO_PMD_OUTPUT);      // motor 220v
	GPIO_SetMode(P1,BIT4,GPIO_PMD_OUTPUT);      // Van 6
	GPIO_SetMode(P0,BIT0,GPIO_PMD_OUTPUT);      // Van 5
	GPIO_SetMode(P4,BIT1,GPIO_PMD_OUTPUT);      // Van 4
	GPIO_SetMode(P0,BIT4,GPIO_PMD_OUTPUT);      // Van 3
	GPIO_SetMode(P0,BIT5,GPIO_PMD_OUTPUT);      // Van 2
	GPIO_SetMode(P0,BIT6,GPIO_PMD_OUTPUT);      // Van 1
	GPIO_SetMode(P0,BIT7,GPIO_PMD_OUTPUT);      // Van Du Phong
	GPIO_SetMode(P2,BIT5,GPIO_PMD_OUTPUT);      // Chuong
	Motor_220V = off;
	Van6 = off;
	Van5 = off;
	Van4 = off;
	Van3 = off;
	Van2 = off;
	Van1 = off;
	VanDP = off;
	Chuong = off;
}


void GPIOP2P3P4_IRQHandler(void)
{
    /* To check if P4.5 interrupt occurred */
    if(GPIO_GET_INT_FLAG(P3, BIT1))
    {
        GPIO_CLR_INT_FLAG(P3, BIT1);
        Xung_CB_LL += 1;
    }
}

void init_interup(void)
{
	GPIO_SetMode(P3, BIT1, GPIO_PMD_INPUT);  // Luu Luong
    GPIO_EnableInt(P3, 1, GPIO_INT_RISING);
    NVIC_EnableIRQ(GPIO_P2P3P4_IRQn);
}

void init_clk_PWM(void)
{
	CLK_EnableModuleClock(PWM67_MODULE);
	CLK_SetModuleClock(PWM67_MODULE, CLK_CLKSEL2_PWM67_S_HIRC, 0);
	SYS_ResetModule(PWM47_RST);
	SYS->P2_MFP &= ~(SYS_MFP_P27_Msk);
 	SYS->P2_MFP |= SYS_MFP_P27_PWM7;
}



