// Header: Thu vien DS1307
// File Name: Giao Tiep DS1307
// Author: Nguyen Xuan Khai
// Date: 1/7/2017

#ifndef _DS1307_C
#define _DS1307_C
#include "i2c_soft.h"
#include "ds1307.h"



unsigned char DS1307_read(unsigned char addr)
{
	unsigned char temp,ret;
	I2C_start(); 		/* Start i2c bus */
	I2C_write(0xD0); 	/* Connect to DS1307 */
	I2C_write(addr); 	/* Request RAM address on DS1307 */
	I2C_start(); 		/* Start i2c bus */
	I2C_write(0XD1); 	/* Connect to DS1307 for Read */
	ret = I2C_read(); 	/* Receive data */
	I2C_stop();
	//**********************************************************
	temp = ret; 							/*BCD to HEX*/
	ret = (((ret/16)*10)+ (temp & 0x0f)); 	/*for Led 7seg*/
	//**********************************************************
	return ret;
}
void DS1307_Write(unsigned char addr,unsigned char dat)
{
	unsigned int temp;
	//********************************************** 
	temp = dat ; 						/*HEX to BCD*/
	dat = (((dat/10)*16)|(temp %10)); 	/*for Led 7seg*/
	//**********************************************
	I2C_start(); 		/* Start i2c bus */
	I2C_write(0XD0); 	/* Connect to DS1307 */
	I2C_write(addr); 	/* Request RAM address on DS1307 */
	I2C_write(dat); 	/* Connect to DS1307 for Read */
	I2C_stop();
}

//xuat xung 1Hz tren chan 7 ds1307
void Out_1Hz()
{
	I2C_start(); 
	I2C_write(0xD0); 
	I2C_write(0x07);   
	I2C_write(0x10);
	I2C_stop();
}	
#endif
