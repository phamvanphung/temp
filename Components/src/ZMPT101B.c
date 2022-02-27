#include <stdio.h>
#include <math.h>
#include "M051Series.h"
#include "ZMPT101B.h"
#include "Delay_sys.h"

extern int i ;

uint16_t Do_ADC_Volt(uint8_t pin,uint8_t channel){
	static uint16_t giaTriADCVolt = 0;
	/* Power on ADC module */
	ADC_POWER_ON(ADC);
	/* Set the ADC operation mode as continuous scan, input mode as single-end and enable the analog input channel 0, 1, 2 and 3 */
	ADC_Open(ADC, ADC_ADCR_DIFFEN_SINGLE_END, ADC_ADCR_ADMD_SINGLE, pin);
	/* clear the A/D interrupt flag for safe */
	ADC_CLR_INT_FLAG(ADC, ADC_ADF_INT);
	/* start A/D conversion */
	ADC_START_CONV(ADC);
	/* Wait conversion done */
	while(!ADC_GET_INT_FLAG(ADC, ADC_ADF_INT));
	/* Stop A/D conversion */
	ADC_STOP_CONV(ADC);
	//i32ConversionData = ADC_GET_CONVERSION_DATA(ADC, u32ChannelCount);
	giaTriADCVolt = ADC_GET_CONVERSION_DATA(ADC,channel);
	/* clear the A/D interrupt flag for safe */
	ADC_CLR_INT_FLAG(ADC, ADC_ADF_INT);
	return giaTriADCVolt;
}

uint16_t calibrate() {
	// static uint16_t acc = 0;
	// static uint16_t i = 0;
	// acc = 0;
	// for ( i = 0; i < 10; i++) {
	// 	acc += Do_ADC_Volt(BIT1,1);
	// }
	// zero = acc / 10;
	// return zero;
}

void setZeroPoint(int _zero) {
//	zero = _zero;
}

float getValueDC(uint8_t adc_bit, uint8_t chane, float sen, uint16_t z) {
	int16_t acc = 0;
	float V = 0.0;
	for (i = 0; i < 10; i++) {
		acc += Do_ADC_Volt(adc_bit, chane) - z;
	}
	V = (float)acc / 10.0 / ADC_SCALE * VREF / sen;
	return V;
}

float getValueAC(uint16_t frequency, uint8_t adc_bit, uint8_t chane, float sen, uint16_t z) {
	uint32_t period = 1000000 / frequency;
	uint32_t t_start = micros();
	float Vrms  = 0.0;
	uint32_t Vsum = 0;
	uint32_t measurements_count = 0;
	int32_t Vnow;
	while (micros() - t_start < period) {
		Vnow = Do_ADC_Volt(adc_bit, chane) - z;
		Vsum += Vnow*Vnow;
		measurements_count++;
	}
  Vrms = sqrt(Vsum / measurements_count) / (ADC_SCALE * VREF) / sen;
	return Vrms;
}


