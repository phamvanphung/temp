#include <stdio.h>
#include "M051Series.h"
#include "clk_init.h"

#define PLLCON_SETTING      CLK_PLLCON_50MHz_HXT
#define PLL_CLOCK           50000000

void SYS_Init(void)
{
		/* Enable Internal RC 22.1184MHz clock */
    CLK_EnableXtalRC(CLK_PWRCON_OSC22M_EN_Msk);

    /* Waiting for Internal RC clock ready */
    CLK_WaitClockReady(CLK_CLKSTATUS_OSC22M_STB_Msk);

    /* Switch HCLK clock source to Internal RC and HCLK source divide 1 */
    CLK_SetHCLK(CLK_CLKSEL0_HCLK_S_HIRC, CLK_CLKDIV_HCLK(1));

    /* Enable external XTAL 12MHz clock */
    CLK_EnableXtalRC(CLK_PWRCON_XTL12M_EN_Msk);

    /* Waiting for external XTAL clock ready */
    CLK_WaitClockReady(CLK_CLKSTATUS_XTL12M_STB_Msk);

    /* Set core clock as PLL_CLOCK from PLL */
    CLK_SetCoreClock(PLL_CLOCK);
		
}

void SYS_Uart1(void){
	 /* Enable UART module clock */
    CLK_EnableModuleClock(UART1_MODULE);
    /* Select UART module clock source */
    CLK_SetModuleClock(UART1_MODULE, CLK_CLKSEL1_UART_S_HIRC, CLK_CLKDIV_UART(1));


    /* Set P1 multi-function pins for UART1 RXD and TXD  */
    SYS->P1_MFP &= ~(SYS_MFP_P12_Msk | SYS_MFP_P13_Msk);
    SYS->P1_MFP |= SYS_MFP_P12_RXD1 | SYS_MFP_P13_TXD1;
}


void I2C_CLK_Source(void)
{
	CLK_EnableModuleClock(I2C1_MODULE);
		/* Configure the SDA0 & SCL0 of I2C0 pins */
	SYS->P4_MFP &= ~(SYS_MFP_P44_Msk | SYS_MFP_P45_Msk);
	SYS->P4_MFP |= (SYS_MFP_P44_SCL1 | SYS_MFP_P45_SDA1);
}

void CLK_Source_TMR(void)
{
	CLK_EnableModuleClock(TMR0_MODULE);
    CLK_SetModuleClock(TMR0_MODULE,CLK_CLKSEL1_TMR0_S_HIRC,1);
}


void ADC_CLK_Source(void)
{
	/* Enable ADC module clock */
	CLK_EnableModuleClock(ADC_MODULE);
	/* Disable gpio pin for ADC*/
	GPIO_DISABLE_DIGITAL_PATH(P1, 0xE3);
	/* ADC clock source is 22.1184MHz, set divider to 7, ADC clock is 22.1184/7 MHz */
	CLK_SetModuleClock(ADC_MODULE, CLK_CLKSEL1_ADC_S_HIRC, CLK_CLKDIV_ADC(7));
	/* Configure the P1.0 - P1.1 P1.5-1.7 ADC analog input pins */
	SYS->P1_MFP &= ~(SYS_MFP_P10_Msk | SYS_MFP_P11_Msk | SYS_MFP_P15_Msk | SYS_MFP_P16_Msk | SYS_MFP_P17_Msk);
	SYS->P1_MFP |= SYS_MFP_P10_AIN0 | SYS_MFP_P11_AIN1 | SYS_MFP_P15_AIN5 | SYS_MFP_P16_AIN6 | SYS_MFP_P17_AIN7 ;
	
}






