#include <stdio.h>
#include "M051Series.h"
#include "i2c.h"
#include "i2c_custom.h"
#include "DS1307.h"


struct
{
 unsigned char s;
 unsigned char m;
 unsigned char h;
 unsigned char dy;
 unsigned char dt;
 unsigned char mt;
 unsigned char yr;
}time;


void DS1307_INIT(void)
{
	MY_I2C_INIT(MyI2C,DS1307_ADDR,100000);
	DS1307_Write(control_reg,0x00);
}

void DS1307_Write(uint8_t addr, uint8_t value)
{
	MY_I2C_START(MyI2C);
	I2C_SET_CONTROL_REG(MyI2C, I2C_I2CON_STA);
	MY_I2C_WRITE(MyI2C,DS1307_WR);
	MY_I2C_WRITE(MyI2C,addr);
	MY_I2C_WRITE(MyI2C,value);
	I2C_SET_CONTROL_REG(MyI2C, I2C_I2CON_STO_SI);
	//MY_I2C_STOP(MyI2C);
}


uint8_t DS1307_READ(uint8_t addr)
{
	uint8_t value = 0x00;
	MY_I2C_START(MyI2C);
	I2C_SET_CONTROL_REG(MyI2C, I2C_I2CON_STA);
	MY_I2C_WRITE(MyI2C,DS1307_WR);
	MY_I2C_WRITE(MyI2C,addr);
	
	MY_I2C_START(MyI2C);
	MY_I2C_WRITE(MyI2C,DS1307_RD);
	value = MY_I2C_READ(MyI2C);
	I2C_SET_CONTROL_REG(MyI2C, I2C_I2CON_STO_SI);
	//MY_I2C_STOP(MyI2C);
	return value;
}


uint8_t bcd_to_decimal(uint8_t value)
{
 return ((value & 0x0F) + (((value & 0xF0) >> 0x04) * 0x0A));
}
uint8_t decimal_to_bcd(uint8_t value)
{
 return (((value / 0x0A) << 0x04) & 0xF0) | ((value % 0x0A) & 0x0F);
}


void DS1307_get_time(void)
{
	time.s = DS1307_READ(sec_reg);
	time.s = bcd_to_decimal(time.s);
	time.m = DS1307_READ(min_reg);
	time.m = bcd_to_decimal(time.m);
	 
	time.h = DS1307_READ(hr_reg);
	time.h = bcd_to_decimal(time.h);

	time.dy = DS1307_READ(day_reg);
	time.dy = bcd_to_decimal(time.dy);

	time.dt = DS1307_READ(date_reg);
	time.dt = bcd_to_decimal(time.dt);

	time.mt = DS1307_READ(month_reg);
	time.mt = bcd_to_decimal(time.mt);

	time.yr = DS1307_READ(year_reg);
	time.yr = bcd_to_decimal(time.yr);

}

void DS1307_set_time(void)
{
 time.s = decimal_to_bcd(time.s);
 DS1307_Write(sec_reg, time.s);
 
 time.m = decimal_to_bcd(time.m);
 DS1307_Write(min_reg, time.m);
 
 time.h = decimal_to_bcd(time.h);
 DS1307_Write(hr_reg, time.h);
 
 time.dy = decimal_to_bcd(time.dy);
 DS1307_Write(day_reg, time.dy);
 
 time.dt = decimal_to_bcd(time.dt);
 DS1307_Write(date_reg, time.dt);
 
 time.mt = decimal_to_bcd(time.mt);
 DS1307_Write(month_reg, time.mt);
 
 time.yr = decimal_to_bcd(time.yr);
 DS1307_Write(year_reg, time.yr);
}







