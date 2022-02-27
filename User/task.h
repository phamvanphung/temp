#include <stdbool.h>
#include "M051Series.h"
void SetUp(void);
void Calculator_ADC(void);
void program(void);
uint8_t * convert_uint16_to_uint8_t(uint16_t array[]);
void Doc_CB_LL (void);
uint16_t DocADC(uint8_t channel);
void Enable_PWM_12V(void);
void Disable_PWM_12V(void);
void PWMA_IRQHandler(void);
void ThoiGianChuTrinh(void);
void LocNuocBinhThuong(void);
void KiemTraSucRua(void);
void SucRuaLoiHai(void);
void SucRuaLoiMot(void);
bool KiemTraDongDien(void);
bool kiemTraDienAp(void);
void cho_Nuoc(void);
void Display_TM1638(void);
void MotNgay(void);
void Check_state(void);
void DocApSuat(void);
