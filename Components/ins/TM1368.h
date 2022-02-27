#include "M051Series.h"
#define PIN_DIO    P13
#define Set_DI		 GPIO_SetMode(P1,BIT3,GPIO_PMD_INPUT)
#define Set_DO 		 GPIO_SetMode(P1,BIT3,GPIO_PMD_OUTPUT)
#define PIN_CLK    P12
#define PIN_STB    P26

void Write1638(unsigned char Data);
void WriteCmd(unsigned char cmd);
unsigned char ReadKey(void);
void WriteData(unsigned char Adress,unsigned char Data);
void LedReset (void);
void WriteDigit(char number,char pos);
void delay_us_soft(unsigned int t);
