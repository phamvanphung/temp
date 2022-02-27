// Header: Thu vien giao tiep I2C
// File Name: I2C
// Author: Nguyen Xuan Khai
// Date: 1/7/2017
#include "M051Series.h"
#include <stdbool.h>



#define SDA P45
#define SCL P44

void i2c_delay_us(unsigned int t)
{
 	while(t--);
}

void I2C_start(void)
{
	SCL = 1;
  i2c_delay_us(1);	
	SCL = 0;
	i2c_delay_us(1);
	SDA = 1;
  i2c_delay_us(1);	
	SCL = 1;
	i2c_delay_us(2);
	SDA = 0; 
	i2c_delay_us(2);
	SCL = 0; 
	i2c_delay_us(2);
}
void I2C_stop(void)
{
	SCL = 1;
  i2c_delay_us(1);	
	SCL = 0;
	i2c_delay_us(1);
	SDA = 0; 
	i2c_delay_us(2);
	SCL = 1; 
	i2c_delay_us(2);
	SDA = 1;
}
bool I2C_write(unsigned char dat)
{
	unsigned char i;
	for (i=0;i<8;i++)
	{
		SDA = (dat & 0x80) ? 1:0;
		SCL=1;
		i2c_delay_us(2);
		SCL=0;
		i2c_delay_us(2);
		dat<<=1;
	}
	SCL = 1; i2c_delay_us(2);
	SCL = 0;
	return dat;
}
unsigned char I2C_read()
{
	unsigned char i, dat;
	dat = 0x00;
	GPIO_SetMode(P4, BIT5, GPIO_PMD_INPUT);
	for(i=0;i<8;i++) 		
	{
		i2c_delay_us(2);
		SCL = 1; 
		i2c_delay_us(2);
		dat = dat * 2;
		if (SDA) dat++;
		//rd_bit = SDA; 		
		//dat = dat<<1;
		//dat = dat | rd_bit;		
		SCL = 0; 			
	}
	GPIO_SetMode(P4, BIT5, GPIO_PMD_OUTPUT);
	return dat;
}

