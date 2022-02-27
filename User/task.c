#include <stdio.h>
#include "include_all_lib.h"
#include "timer.h"
#include "gpio.h"
#include "ZMPT101B.h"
#include "TM1368.h"
#include "adc.h"

/*------------- global variable ------------*/


uint8_t time_array[12] = {0};

/* ADC variables*/
// _Bool read_ADC_finish = false;

uint8_t timeCheckLuuLuong = 0;
uint16_t Xung_CB_LL = 0;
float HesoSanXuat = 4.8;
uint8_t HesoCalibLuuLuong = 100;
float LuuLuong = 0.0;
uint16_t TanSoLuuLuong = 0;

uint8_t HeSoCalibLuuLuongTM1638 = 100;
uint8_t HeSoCalibApSuatTM1638 = 100;


uint8_t HeSoCalibApSuat = 100;
uint32_t ApSuat = 0;		// psi
uint16_t ADC_ApSuat = 0;
float DongApSuat = 0;
uint32_t ApSuatSum = 0;
uint16_t timeApSuat = 0;

/*------------------------------*/
uint8_t tansoPWM = 50;

bool flagSucRuaLoiMot = false;
bool flagSucRuaLoiHai = false;

uint8_t soLanBom = 0;
bool coLoi = false;
bool coDangBaoVe = false;
uint8_t stepLocNuocBinhThuong = 0;
uint16_t ThoigianMoMayLocNuoc = 0;
uint16_t ThoigianTatMayLocNuoc = 0;

uint8_t stepChoNuoc = 0;
uint16_t ThoiGianBomChoNuoc = 0;
uint16_t ThoiGianTatBomChoNuoc = 0;

float TongLuongNuoc = 0;

float luongSucRuaLoi2 = 0;
uint8_t SoNgayLocLoi2 = 3;			// PLc cai Dat
uint8_t NgayLocLoi2 = 0;			// lay tu RealTime
uint8_t gioHenLoi2 = 10;
uint8_t phutHenLoi2 = 10;
uint8_t StepSucRuaLoiHai = 0;
uint16_t ThoigianXaNguocLoi2 = 000;
uint16_t ThoigianXaThuanLoi2 = 000;

float luongSucRuaLoi1 = 0;
uint8_t SoNgayLocLoi1 = 3; // PLc Cai Dat
uint8_t NgayLocLoi1 = 0 ;	// Lay Thu RealTime
uint8_t gioHenLoi1 = 11;
uint8_t phutHenLoi1 = 11;
uint8_t StepSucRuaLoiMot = 0;
uint16_t ThoigianSucRuaLoiMot = 0;



bool DaCongMotNgay = false;

uint16_t i = 0;
/*------------------------------------------*/
bool PhaoNuocDaDay = true;
uint8_t thoigianPhaoNuoc = 0;

/*---------------- INIT --------------------*/

/*---------------- DS1307 ------------------*/
uint8_t gio = 0;
uint8_t phut = 0;
uint8_t giay = 0;

uint8_t day = 0;
uint8_t month = 0;
uint8_t year =0;

/*----------------- TM1638 -------------------*/
unsigned char readKey = 0;
unsigned char state = 0;
float DongDien =0;
float DienAp = 0;
uint32_t volt_sum_ac = 0;
uint32_t current_sum_ac = 0;
float sensitivity_volt  = 0.0000782;
float sensitivity_current = 0.0000043;
uint16_t zero  = 2048;



void SetUp(void)
{
	//UART_Open(UART1,115200);
	/*---------------- Set up Timer interup -----*/
	TimerTickInit();
	Set_sys_init();
	init_interup(); // ngat luu luong
	// DS 1307
	Out_1Hz();
	DS1307_Write(HOUR, 14);
	DS1307_Write(MIN, 46);
	DS1307_Write(SEC, 0);
	DS1307_Write(DAY, 20);
	DS1307_Write(MONTH, 2);
	DS1307_Write(YEAR, 22);

	gio = DS1307_read(HOUR);
	phut = DS1307_read(MIN);
	giay = DS1307_read(SEC);
	//
	time_array[0] = gio /10 % 10 + 48;
	time_array[1] = gio  % 10 + 48;
	time_array[2] = phut /10 % 10 + 48;
	time_array[3] = phut  % 10 + 48;
	time_array[4] = giay /10 % 10 + 48;
	time_array[5] = giay  % 10 + 48;
	
	NgayLocLoi2 = DS1307_read(0x08); // So ngay loc Loi 2;
	NgayLocLoi1 = DS1307_read(0x09); // so ngay loc loi 1
	if(DS1307_read(0x0A)== 0){
		HesoCalibLuuLuong = 100;
	} else {
		HesoCalibLuuLuong = DS1307_read(0x0A);
	}

	if(DS1307_read(0x0B)== 0){
		HeSoCalibApSuat = 100;
	} else {
		HeSoCalibApSuat = DS1307_read(0x0B);
	}

	// init PWM	(ENABLE-DISABLE)
	// INIT ADC (DocADC Se init)

	/*---------------- Set up ADC - GPIO ---------------*/
	GPIO_Init();
	/*---------------- TM1638 --------------------------*/
	GPIO_SetMode(P2,BIT6,GPIO_PMD_OUTPUT); // stb
  	GPIO_SetMode(P1,BIT3,GPIO_PMD_OUTPUT); // dto
  	GPIO_SetMode(P1,BIT2,GPIO_PMD_OUTPUT); // clk
	LedReset();
	/*Test */


}

/* in loop */
void program(void)
{
	
	// chuong trinh tong:
	if(kiemTraDienAp()){
		while (kiemTraDienAp())
		{
			Check_state();
			Display_TM1638();
			DocApSuat();
			delay_ms(100);
		}
	}
	if(KiemTraDongDien()){
		while (1)
		{
			Check_state();
			Display_TM1638();
			DocApSuat();
			Chuong = on;
		}
		
	}
	
	/* Kiem tra co vao chuong trinh suc rua */
	Check_state();
	DocApSuat();
	Display_TM1638();
	MotNgay();
	KiemTraSucRua();
	if(flagSucRuaLoiMot == true)
	{
		// Chuong Trinh Suc Rua Loi Mot
		while (flagSucRuaLoiHai)
		{
			SucRuaLoiMot();
			Check_state();
			Display_TM1638();
			DocApSuat();
		}
	}
	
	if (flagSucRuaLoiHai == true)
	{
		// Chuong Trinh Suc Rua Loi Hai
		while (flagSucRuaLoiHai)
		{
			SucRuaLoiHai();
			Check_state();
			Display_TM1638();
			DocApSuat();
		}
	}
	// /* Loc Nuoc Binh Thuong*/
	if(PhaoNuocDaDay == false){
		cho_Nuoc();
		LocNuocBinhThuong();
	} else if(PhaoNuocDaDay == true) {
		stepChoNuoc = 0;
		stepLocNuocBinhThuong = 0;
		soLanBom = 0;
		coLoi = 0;
		Van1 = off;
		Van2 = off;
		Van3 = off;
		Van4 = off;
		Van5 = off;
		Van6 = off;
		VanDP = off;
		Motor_220V = off;
	}
	
	if(soLanBom >= soLanBomLonNhat){
		while (1)
		{
			Van1 = off;
			Van2 = off;
			Van3 = off;
			Van4 = off;
			Van5 = off;
			Van6 = off;
			VanDP = off;
			Motor_220V = off;
			Chuong = on;
		}
	}
}

/*
* step = 0 : bo qua loc nuoc Binh Thuong
* step = 1: mo may loc nuoc binh thuong
* step = 2: Tat may loc nuoc binh thuong de nghi bao ve motor
* Khi dang Loc loi 1, Loc Loi 2. Step = 0;
*/

void LocNuocBinhThuong(void)
{
	switch (stepLocNuocBinhThuong)
	{
		case 0x01:
		{
			/* code */
			Van1 = on;
			VanDP = on;
			Van2 = on;
			Van3 = on;
			Van4 = off;
			Van5 = off;
			Van6 = off;
			Motor_220V = on;
			if(TanSoLuuLuong <= giaTriKhongCoNuoc)
			{
				stepLocNuocBinhThuong = 0;
				stepChoNuoc = 0;
			}
			break;
		}
		case 0x02:
		{
			Van1 = off;
			Van2 = off;
			Van3 = off;
			VanDP = off;
			Van4 = off;
			Van5 = off;
			Van6 = off;
			Motor_220V = off;
			break;
		}
		default:
			break;
	}
	
}


void cho_Nuoc(void){
	switch (stepChoNuoc)
	{
	case 0x01:
	{
			Van1 = on;
			VanDP = on;
			Van2 = on;
			Van3 = on;
			Van4 = off;
			Van5 = off;
			Van6 = off;
			Motor_220V = on;
			break;
	}
	case 0x02:
	{
			Van1 = off;
			Van2 = off;
			Van3 = off;
			VanDP = off;
			Van4 = off;
			Van5 = off;
			Van6 = off;
			Motor_220V = off;
			break;
	}
	default:
		break;
	}
}

void ThoiGianChuTrinh(void)
{
	
	if((PhaoBonNuoc == bonHo)&&(PhaoNuocDaDay == true)){
		thoigianPhaoNuoc+= 1;
		if(thoigianPhaoNuoc > 5){
			PhaoNuocDaDay = false;
			thoigianPhaoNuoc = 0;
		}
	}
	if((PhaoBonNuoc == bonDay)&&(PhaoNuocDaDay == false)){
		thoigianPhaoNuoc+= 1;
		if(thoigianPhaoNuoc > 5){
			PhaoNuocDaDay = true;
			thoigianPhaoNuoc = 0;
		}
	}

	if((flagSucRuaLoiHai == false)&&(flagSucRuaLoiMot == false))
	{
		if(PhaoBonNuoc == bonHo)
		{
			if(stepLocNuocBinhThuong == 0 && (TanSoLuuLuong > giaTriKhongCoNuoc))
			{
				stepLocNuocBinhThuong = 1;
				stepChoNuoc = 0;
			}
			if(stepLocNuocBinhThuong == 0x01  && (TanSoLuuLuong > giaTriKhongCoNuoc)){
				ThoigianMoMayLocNuoc += 1;
				stepChoNuoc = 0;
				if(ThoigianMoMayLocNuoc > thoiGianMotLanBom)
				{
					soLanBom += 1;
					stepLocNuocBinhThuong  = 0x02;
					ThoigianTatMayLocNuoc = 0;
					coDangBaoVe = true;
				}
			}
			if(stepLocNuocBinhThuong == 0x02){
				ThoigianTatMayLocNuoc += 1;
				if(ThoigianTatMayLocNuoc > thoiGianBaoVe){
					stepLocNuocBinhThuong  = 0x01;
					ThoigianMoMayLocNuoc = 0;
				}
			}
			if(((stepLocNuocBinhThuong == 0)&&(TanSoLuuLuong <= giaTriKhongCoNuoc)) || ((stepLocNuocBinhThuong == 1)&&(TanSoLuuLuong <= giaTriKhongCoNuoc)))
			{
				if(stepChoNuoc == 0x00){
					stepChoNuoc = 1;
				}
				if(stepChoNuoc == 0x01){
					// UART_WRITE(UART1,50);
					// UART_WRITE(UART1,10);
					ThoiGianBomChoNuoc += 1;
					if(ThoiGianBomChoNuoc > thoiGianChoNuoc){
						ThoiGianTatBomChoNuoc = 0;
						stepChoNuoc = 0x02;
					}
				} else if(stepChoNuoc == 0x02){
					ThoiGianTatBomChoNuoc += 1;
					if(ThoiGianTatBomChoNuoc > thoiGianTatChoNuoc){
						stepChoNuoc = 0x01;
						ThoiGianBomChoNuoc = 0;
					}
				}
			}

		}
	}

	if((flagSucRuaLoiHai == true)&&(flagSucRuaLoiMot == false)){
		if(StepSucRuaLoiHai == 0){
			StepSucRuaLoiHai = 1;
		}
		if(StepSucRuaLoiHai == 1){
			ThoigianXaNguocLoi2 += 1;
			if(ThoigianXaNguocLoi2 > thoiGianXaNguocLoi2CaiDat){
				StepSucRuaLoiHai = 2;
				ThoigianXaThuanLoi2 = 0;
			}
		} else if(StepSucRuaLoiHai == 2){
			ThoigianXaThuanLoi2 += 1;
			if(ThoigianXaThuanLoi2 > thoiGianXaThuanLoi2CaiDat){
				StepSucRuaLoiHai = 3;
				ThoigianXaNguocLoi2 = 0;
			}
		}
	}

	if(flagSucRuaLoiMot == true){
		if(StepSucRuaLoiMot == 0){
			StepSucRuaLoiMot = 1;
		} else if(StepSucRuaLoiMot == 1){
			ThoigianSucRuaLoiMot += 1;
			if(ThoigianSucRuaLoiMot > thoiGianSucRuaLoiMotCaiDat)
			{
				StepSucRuaLoiMot = 2;
				ThoigianSucRuaLoiMot = 0;
			}
		}
	}
}


void Calculator_ADC(void)
{
	/*---------- Calculator ADC 220V ---------*/
	
	/*---------- Calculator ADC Current ------*/
	
	/*---------- Calculator ADC 24V ----------*/
	
	/*---------- Calculator ADC 12V FB -------*/
	
	/*---------- Calculator ADC Cam Bien -----*/
}

uint8_t * convert_uint16_to_uint8_t(uint16_t array[])
{
	static uint8_t arra[] = {0};
	size_t n = sizeof(array)/sizeof(array[0]);
	for( i = 0; i < n ; i++){
		arra[2*i]= array[i] & 0xff;
		arra[2*i+1]=(array[i] >> 8);
	}
	return arra;
}

void SucRuaLoiMot(void)
{
	switch (StepSucRuaLoiMot)
	{
		case 1:
		{
			Van1 = on;
			Van2 = on;
			Van3 = off;
			Van4 = off;
			Van5 = off;
			Van6 = off;
			VanDP = on;
			Motor_220V = on;
			break;
		}
		case 2:
		{
			Van1 = off;
			Van2 = off;
			Van3 = off;
			Van4 = off;
			Van5 = off;
			Van6 = off;
			VanDP = off;
			Motor_220V = off;
			flagSucRuaLoiMot = false;
			NgayLocLoi1 = 0;
			luongSucRuaLoi1 = 0;
			StepSucRuaLoiMot = 0;
			DS1307_Write(0x09,0);
			break;
		}
	}
}


uint16_t giaTriADCVolt = 0;
void DocApSuat(void){
	for(i = 0 ; i < 20; i++)
	{
	/* Power on ADC module */
		ADC_POWER_ON(ADC);
		/* Set the ADC operation mode as continuous scan, input mode as single-end and enable the analog input channel 0, 1, 2 and 3 */
		ADC_Open(ADC, ADC_ADCR_DIFFEN_SINGLE_END, ADC_ADCR_ADMD_SINGLE, BIT7);
		/* clear the A/D interrupt flag for safe */
		ADC_CLR_INT_FLAG(ADC, ADC_ADF_INT);
		/* start A/D conversion */
		ADC_START_CONV(ADC);
		/* Wait conversion done */
		while(!ADC_GET_INT_FLAG(ADC, ADC_ADF_INT));
		/* Stop A/D conversion */
		ADC_STOP_CONV(ADC);
		//i32ConversionData = ADC_GET_CONVERSION_DATA(ADC, u32ChannelCount);
		giaTriADCVolt = ADC_GET_CONVERSION_DATA(ADC,7);
		/* clear the A/D interrupt flag for safe */
		ADC_CLR_INT_FLAG(ADC, ADC_ADF_INT);

		if(giaTriADCVolt > 700)
		{
			DongApSuat = 14*(0.020464793 * giaTriADCVolt - 13 ) * (HeSoCalibApSuat/100.0) ;
		} else {
			DongApSuat = 0;
		}
		ApSuatSum += (uint32_t)DongApSuat;
	}
		ApSuat = ApSuatSum / 20;
		timeApSuat = 0;
		ApSuatSum = 0;
}

void SucRuaLoiHai(void)
{
	switch(StepSucRuaLoiHai)
	{
		case 0x01:
		{
			Van1 = off;
			Van2 = off;
			Van3 = off;
			VanDP = off;
			Van5 = on;
			Motor_220V = on;
			Van6 = on;
			Van4 = on;
			break;
		}
		case 0x02:
		{
			Van5 = on;
			Motor_220V = on;
			Van6 = off;
			Van1 = off;
			Van2 = on;
			Van3 = off;
			Van4 = on;
			VanDP = off;
			break;
		}
		case 0x03:
		{
			flagSucRuaLoiHai = false;
			NgayLocLoi2 = 0;
			DS1307_Write(0x08,0);
			StepSucRuaLoiHai = 0;
			luongSucRuaLoi2 =0;
			break;
		}
	}
}

uint16_t DocADC(uint8_t channel)
{
	/* chuong trinh doc ADC theo channel*/
	ADC_POWER_ON(ADC);
	switch(channel){
		case 0x01:	// Doc ADC Vin
		{
			ADC_Open(ADC, ADC_ADCR_DIFFEN_SINGLE_END, ADC_ADCR_ADMD_SINGLE,0x1 << 5);
			ADC_CLR_INT_FLAG(ADC, ADC_ADF_INT);
			ADC_START_CONV(ADC);
			while(!ADC_GET_INT_FLAG(ADC, ADC_ADF_INT));
			ADC_STOP_CONV(ADC);
			ADC_CLR_INT_FLAG(ADC, ADC_ADF_INT);
			return ADC_GET_CONVERSION_DATA(ADC,5);
		}
		case 0x02:	// Doc ADC Vout
		{
			ADC_Open(ADC, ADC_ADCR_DIFFEN_SINGLE_END, ADC_ADCR_ADMD_SINGLE,0x1 << 6);
			ADC_CLR_INT_FLAG(ADC, ADC_ADF_INT);
			ADC_START_CONV(ADC);
			while(!ADC_GET_INT_FLAG(ADC, ADC_ADF_INT));
			ADC_STOP_CONV(ADC);
			ADC_CLR_INT_FLAG(ADC, ADC_ADF_INT);
			return ADC_GET_CONVERSION_DATA(ADC,6);
		}
		case 0x03:	// Doc ADC 220V
		{
			ADC_Open(ADC, ADC_ADCR_DIFFEN_SINGLE_END, ADC_ADCR_ADMD_SINGLE,0x1 << 0);
			ADC_CLR_INT_FLAG(ADC, ADC_ADF_INT);
			ADC_START_CONV(ADC);
			while(!ADC_GET_INT_FLAG(ADC, ADC_ADF_INT));
			ADC_STOP_CONV(ADC);
			ADC_CLR_INT_FLAG(ADC, ADC_ADF_INT);
			return ADC_GET_CONVERSION_DATA(ADC,0);
		}
		case 0x04:	// Doc ADC Dong Dien
		{
			ADC_Open(ADC, ADC_ADCR_DIFFEN_SINGLE_END, ADC_ADCR_ADMD_SINGLE,0x1 << 1);
			ADC_CLR_INT_FLAG(ADC, ADC_ADF_INT);
			ADC_START_CONV(ADC);
			while(!ADC_GET_INT_FLAG(ADC, ADC_ADF_INT));
			ADC_STOP_CONV(ADC);
			ADC_CLR_INT_FLAG(ADC, ADC_ADF_INT);
			return ADC_GET_CONVERSION_DATA(ADC,1);
		}
		case 0x05: // Doc ADC Ap Suat
		{
			ADC_Open(ADC, ADC_ADCR_DIFFEN_SINGLE_END, ADC_ADCR_ADMD_SINGLE,0x1 << 7);
			ADC_CLR_INT_FLAG(ADC, ADC_ADF_INT);
			ADC_START_CONV(ADC);
			while(!ADC_GET_INT_FLAG(ADC, ADC_ADF_INT));
			ADC_STOP_CONV(ADC);
			ADC_CLR_INT_FLAG(ADC, ADC_ADF_INT);
			return ADC_GET_CONVERSION_DATA(ADC,7);
		}
	}
}




void Doc_CB_LL (void)
{
	TanSoLuuLuong = Xung_CB_LL;
	LuuLuong = (Xung_CB_LL / HesoSanXuat) * ((HesoCalibLuuLuong+10)/100.0);  // he so luu luong = 10.anpha.
	luongSucRuaLoi2 = LuuLuong /60 + luongSucRuaLoi2 ;
	Xung_CB_LL = 0;
}

void Enable_PWM_12V(void)
{
	SYS->P2_MFP &= ~(SYS_MFP_P27_Msk);
 	SYS->P2_MFP |= SYS_MFP_P27_PWM7;
	PWM_EnableOutput(PWMB, 0x8);
	PWM_ConfigOutputChannel(PWMB, PWM_CH3, 500000, tansoPWM);
	/* Enable Timer period Interrupt */
   PWM_EnablePeriodInt(PWMB, PWM_CH3, PWM_PERIOD_INT_UNDERFLOW);
	NVIC_EnableIRQ((IRQn_Type)(PWMB_IRQn));
	PWM_Start(PWMB,0x8);
}

void Disable_PWM_12V(void)
{
	PWMB->CNR3 = 0;
	NVIC_DisableIRQ((IRQn_Type)(PWMB_IRQn));
	while(PWMB->PDR3 != 0); 
	/* Disable the PWM Timer */
	PWM_Stop(PWMB, 0x8);
	/* Disable PWM Output pin */
	PWM_DisableOutput(PWMB, 0x8);
	
	SYS->P2_MFP &= ~(SYS_MFP_P27_Msk);
  	SYS->P2_MFP |= SYS_MFP_P27_GPIO;
	GPIO_SetMode(P2,BIT7,GPIO_PMD_OUTPUT);
	P27 = 0;
}


void PWMB_IRQHandler(void)
{
    uint32_t u32PwmIntFlag;
    /* Handle PWMA Timer function */
    u32PwmIntFlag = PWMB->PIIR;
    /* PWMB channel 0 PWM timer interrupt */
    if(u32PwmIntFlag & PWM_PIIR_PWMIF3_Msk)
    {
        PWMB->PIIR = PWM_PIIR_PWMIF3_Msk;
    }
}


void MotNgay(void){
	gio = DS1307_read(HOUR);
	if((gio == 1)&&(DaCongMotNgay == false)){
		NgayLocLoi2 += 1;
		NgayLocLoi1 += 1;
		DaCongMotNgay = true;
		DS1307_Write(0x08,NgayLocLoi2);	// tinh so ngay loc loi 2
		DS1307_Write(0x09,NgayLocLoi1); // tinh so ngay loc loi 1
	}
	if(gio == 2){
		DaCongMotNgay = false;
	}
}

void KiemTraSucRua(void)
{
	// kiem tra suc rua loi 2
	if(luongSucRuaLoi2 > LuongNuocSucRuaLoi2)
	{
		flagSucRuaLoiHai = true;
	}
	if(NgayLocLoi2 >= SoNgayLocLoi2){
		if((DS1307_read(HOUR)== gioHenLoi2)&&(DS1307_read(MIN) == phutHenLoi2)){
			flagSucRuaLoiHai = true;
		}
	}
	// kiem tra suc rua loi 1
	if(luongSucRuaLoi1 > LuongNuocSucRuaLoi1)
	{
		flagSucRuaLoiMot = true;
	}
	if(NgayLocLoi1 >= SoNgayLocLoi1){
		if((DS1307_read(HOUR) == gioHenLoi1) && (DS1307_read(MIN) == phutHenLoi1))
		{
			flagSucRuaLoiMot = true;
		}
	}
}

bool kiemTraDienAp(void)
{
	for(i = 0; i< 10; i ++){
		DienAp = getValueAC(50, volt_bit, volt_chane, sensitivity_volt, zero);
		if (DienAp < 20) {
			DienAp = 0;
		}			
		volt_sum_ac += (uint32_t) DienAp;
	}
	volt_sum_ac = volt_sum_ac / 10;
	if((volt_sum_ac > 250)||(volt_sum_ac < 180)){
		return true;
	} else {
		return false;
	}
}
bool KiemTraDongDien(void)
{
	for(i=0;i<10;i++){
		DongDien = getValueAC(50, current_bit, current_chane, sensitivity_current, zero);		
		if (DongDien < 160) {
			DongDien = 0;
		}	
		current_sum_ac += (uint32_t) DongDien;
	}
	current_sum_ac = current_sum_ac /10;
	if(current_sum_ac > 4000){
		return true;
	} else {
		return false;
	}
}

void Check_state(void)
{
	unsigned char check ;
	check = ReadKey();
	if(check == 0x1F)
	{
		state = 0;
	} else if(check == 0x17){
		state = 1;
	} else if(check == 0x0F){
		state = 2;
	} else if(check == 0x07)
	{
		state = 3;
	} else if(check == 0x1B){
		// tang
		if(state == 2){
			HeSoCalibLuuLuongTM1638 += 1;
		} else if(state == 3){
			HeSoCalibApSuatTM1638 += 1;
		}
		while(ReadKey()== 0x1B);
	} else if(check == 0x13){
		// giam
		if(state == 2){
			HeSoCalibLuuLuongTM1638 -= 1;
		} else if(state == 3){
			HeSoCalibApSuatTM1638 -= 1;
		}
		while(ReadKey()== 0x13);
	} else if(check == 0x0B){
		// set
		HesoCalibLuuLuong = HeSoCalibLuuLuongTM1638;
		HeSoCalibApSuat = HeSoCalibApSuatTM1638;
		DS1307_Write(0x0A,HesoCalibLuuLuong);
		DS1307_Write(0x0B,HeSoCalibApSuat);
	}
}

void Display_TM1638(void){
	uint16_t display = 0;
	switch (state)
	{
	case 0: 	// hien thi Dong Dien
	{
		display = current_sum_ac;
		WriteDigit(display/1000%10,8);
		WriteDigit(display/100%10,10);
		WriteDigit(display/10%10,12);
		WriteDigit(display%10,14);

		WriteDigit(18,0);
		WriteDigit(18,2);
		WriteDigit(18 ,4);
		WriteDigit(18,6);

		break;
	}
	case 1:
	{
		display = volt_sum_ac;
		WriteDigit(display/1000%10,8);
		WriteDigit(display/100%10,10);
		WriteDigit(display/10%10,12);
		WriteDigit(display%10,14);

		WriteDigit(18,0);
		WriteDigit(18,2);
		WriteDigit(18 ,4);
		WriteDigit(18,6);
		break;
	}
	case 2:		// calip luu luong
	{
		display = HeSoCalibLuuLuongTM1638;
		// WriteDigit(display/1000%10,8);
		WriteDigit(18,8);
		WriteDigit(display/100%10,10);
		WriteDigit((display/10%10),12);
		WriteDigit(display%10,14);
		
		display = LuuLuong;
		//WriteDigit(display/1000%10,0);
		WriteDigit(display/1000%10,0);
		WriteDigit(display/100%10,2);
		WriteDigit((display/10%10) ,4);
		WriteDigit(display%10,6);
		break;
	}
	case 3:		// calip ap Suat
	{
		display = HeSoCalibApSuatTM1638;
		//WriteDigit(display/1000%10,8);
		WriteDigit(18,8);
		WriteDigit(display/100%10,10);
		WriteDigit((display/10%10),12);
		WriteDigit(display%10,14);


		display = ApSuat;
		WriteDigit(display/1000%10,0);
		WriteDigit(display/100%10,2);
		WriteDigit((display/10%10),4);
		WriteDigit(display%10,6);
		break;
	}
	default:
		break;
	}
}

