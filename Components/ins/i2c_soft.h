#include <stdbool.h>
void i2c_delay_us(unsigned int t);
void I2C_start(void);
void I2C_stop(void);
bool I2C_write(unsigned char dat);
unsigned char I2C_read();
