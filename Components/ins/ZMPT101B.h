#include <stdio.h>
#include "M051Series.h"

#define ADC_SCALE 4095
#define VREF 5.0
#define DEFAULT_FREQUENCY 50


#define volt_bit BIT1
#define volt_chane 1
#define current_bit BIT0
#define current_chane 0



float getVoltageAC(uint16_t frequency);
float getVoltageDC(void);
void setSensitivity(float sens);
void setZeroPoint(int _zero);
uint16_t calibrate(void);
uint16_t Do_ADC_Volt(uint8_t pin,uint8_t channel);
float getValueAC(uint16_t frequency, uint8_t adc_bit, uint8_t chane, float sen, uint16_t z);
