#include <stdio.h>
#include "M051Series.h"


uint8_t MY_I2C_READ(I2C_T *i2c);
void MY_I2C_WRITE(I2C_T *i2c,uint8_t value);
void MY_I2C_INIT(I2C_T *i2c, uint8_t addrDevice, uint32_t clk);
void MY_I2C_START(I2C_T *i2c);
void MY_I2C_CLOSE(I2C_T *i2c);
void MY_I2C_STOP(I2C_T *i2c);

