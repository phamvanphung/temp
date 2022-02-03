#include "M051Series.h"

#define MyI2C I2C1

#define I2C_W 0
#define I2C_R 1
#define sec_reg 0x00
#define min_reg 0x01
#define hr_reg 0x02
#define day_reg 0x03
#define date_reg 0x04
#define month_reg 0x05
#define year_reg 0x06
#define control_reg 0x07

#define DS1307_ADDR 0xD0
#define DS1307_WR (DS1307_ADDR + I2C_W)
#define DS1307_RD (DS1307_ADDR + I2C_R)

void DS1307_INIT(void);
void DS1307_Write(uint8_t addr, uint8_t value);
uint8_t DS1307_READ(uint8_t addr);
void DS1307_get_time(void);
void DS1307_set_time(void);