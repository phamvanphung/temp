#include <stdio.h>
#include "include_all_lib.h"
#include "timer.h"
#include "gpio.h"

/*------------- global variable ------------*/
uint8_t time_array[6] = {0};
uint16_t adc_array[5] = {0}; // a[0]: AC current, a[1]: DC current, a[2]:ADC 24V, a[3]: ADC FB 12V,a[4]: CB AP SUAT
/* ADC variables*/
_Bool read_ADC_finish = false;
// uint16_t Value_ADC_220V = 100;
// uint16_t ADC_CURENT = 1;
// uint16_t ADC_24V = 10;
// uint16_t ADC_12V_FB = 10;
// uint16_t ADC_CB_APSUAT = 1;

uint16_t Xung_CB_LL = 0;
float HesoSanXuat = 4.8;
float HesoCalib = 1.0;
float LuuLuong = 0.0;

uint16_t TanSoLuuLuong = 0;

uint8_t tansoPWM = 50;

bool flagSucRuaLoiMot = false;
bool flagSucRuaLoiHai = false;

uint8_t soLanBaoVe = 0;
bool coLoi = false;
bool coDangBaoVe = false;
uint8_t stepLocNuocBinhThuong = 0;
uint16_t ThoigianMoMayLocNuoc = 0;
uint16_t ThoigianTatMayLocNuoc = 0;


uint32_t thoigianSucRuaLoiHai = 1000; // tinh 
float LuuluongSucRuaLoi2 = 00;
float TongLuongNuoc = 0;

uint8_t StepSucRuaLoiHai = 1;
uint8_t ThoigianXaNguocLoi2 = 000;
uint8_t ThoigianXaThuanLoi2 = 000;


uint16_t i = 0;
/*---------------- INIT --------------------*/

/*---------------- DS1307 ------------------*/
extern struct
{
 unsigned char s;
 unsigned char m;
 unsigned char h;
 unsigned char dy;
 unsigned char dt;
 unsigned char mt;
 unsigned char yr;
}time;

void SetUp(void)
{
	/*---------------- Set up DS1307 -----------*/
	// DS1307_INIT();
	// time_array[3] = 0x15;
	// UART_Write(UART1,time_array + 3,1);
	// time.s = 0;
	// time.m = 49;
	// time.h = 13;
	// time.dy = 3;
	// time.mt = 2;
	// time.yr = 21;
	// DS1307_set_time();
	// DS1307_get_time();
	// time_array[0] = time.s;
	// time_array[1] = time.m;
	// time_array[2] = time.h;
	// UART_Write(UART1,time_array,3);
	/*---------------- Set up Timer interup -----*/
	TimerTickInit();
	Set_sys_init();
	/*---------------- Set up ADC - GPIO ---------------*/
	GPIO_Init();

	/*Test */
	 Enable_PWM_12V();
	 delay_ms(2000);
	 Disable_PWM_12V();
	 delay_ms(500);
	 Enable_PWM_12V();
	 delay_ms(3000);
	 Disable_PWM_12V();

}

/* in loop */
void program(void)
{
	// chuong trinh tong:
	/* Kiem tra co vao chuong trinh suc rua */
	KiemTraSucRua();
	if(flagSucRuaLoiMot == true)
	{
		// Chuong Trinh Suc Rua Loi Mot
	}
	if (flagSucRuaLoiHai == true)
	{
		// Chuong Trinh Suc Rua Loi Hai
		while (flagSucRuaLoiHai)
		{
			SucRuaLoiHai();
		}
	}
	/* Loc Nuoc Binh Thuong*/
	LocNuocBinhThuong();

}



void LocNuocBinhThuong(void)
{
	if(PhaoIn == off)
	{
		if((TanSoLuuLuong <= giaTriKhongCoNuoc)&&(coDangBaoVe == false)) // Khong co nuoc
		{
			Van1 = on;
			VanDP = on;
			Van2 = on;
			Van3 = on;
			Motor_220V = on;
			kiemTraDienAp();
			KiemTraDongDien();
			delay_ms(10000);
			if(TanSoLuuLuong <=  giaTriKhongCoNuoc)
			{
				Van1 = off;
				VanDP = off;
				Van2 = off;
				Van3 = off;
				Motor_220V = off;
				stepLocNuocBinhThuong = 0;
				coLoi = true;
				for(i = 0;i < 120; i++)
				{
					delay_ms(1000);
				}
				return ;
			} else 
			{
				coLoi = false;
			}
			if(stepLocNuocBinhThuong == 0)
			{
				coLoi = false;
				stepLocNuocBinhThuong = 1;
			}
		} 
		// Co nuoc
		switch (stepLocNuocBinhThuong)
		{
			case 1 : // loc lan 1:
			{
				// Khoi Dong may Kiem tra co nuoc
				Van1 = on;
				VanDP = on;
				Van2 = on;
				Van3 = on;
				Motor_220V = on;
				kiemTraDienAp();
				KiemTraDongDien();
				coDangBaoVe = false;
				break;
			}
			case 2:
			{
				Van1 = off;
				VanDP = off;
				Van2 = off;
				Van3 = off;
				Motor_220V = off;
				kiemTraDienAp();
				KiemTraDongDien();
				soLanBaoVe = 1;
				coDangBaoVe = true;
				break;
			}
			case 3:
			{
				Van1 = on;
				VanDP = on;
				Van2 = on;
				Van3 = on;
				Motor_220V = on;
				kiemTraDienAp();
				KiemTraDongDien();
				coDangBaoVe = false;
				break;
			}
			case 4:
			{
				Van1 = off;
				VanDP = off;
				Van2 = off;
				Van3 = off;
				Motor_220V = off;
				kiemTraDienAp();
				KiemTraDongDien();
				soLanBaoVe = 2;
				coDangBaoVe = true;
				break;
			}
			case 5:
			{
				Van1 = on;
				VanDP = on;
				Van2 = on;
				Van3 = on;
				Motor_220V = on;
				kiemTraDienAp();
				KiemTraDongDien();
				coDangBaoVe = false;
				break;
			}
			case 6:
			{
				Van1 = off;
				VanDP = off;
				Van2 = off;
				Van3 = off;
				Motor_220V = off;
				kiemTraDienAp();
				KiemTraDongDien();
				soLanBaoVe = 3;
				coDangBaoVe = true;
				coLoi = true;
				break;
			}
		}
	} else
	{
		stepLocNuocBinhThuong = 0;
		ThoigianMoMayLocNuoc = 0;
		ThoigianTatMayLocNuoc = 0;
		soLanBaoVe = 0;
		coDangBaoVe = false;
		coLoi = false;
	}

}


void ThoiGianChuTrinh(void)
{
	if(flagSucRuaLoiHai == true)
	{
		if(StepSucRuaLoiHai == 1)
		{
			ThoigianXaNguocLoi2 += 1;
			if(ThoigianXaNguocLoi2 > 600)
			{
				ThoigianXaNguocLoi2 = 0;
				StepSucRuaLoiHai = 2;
			}
		} else if(StepSucRuaLoiHai == 2) {
			ThoigianXaThuanLoi2 += 1;
			if(ThoigianXaThuanLoi2 > 600)
			{
				ThoigianXaThuanLoi2 = 0;
				StepSucRuaLoiHai = 3;
			}
		}
	}
	if((PhaoIn == off)&&(flagSucRuaLoiMot == false)&&(flagSucRuaLoiHai == false)&&(coLoi == false))
	{
		if((stepLocNuocBinhThuong == 1))
		{
			ThoigianTatMayLocNuoc = 0;
			ThoigianMoMayLocNuoc += 1;
			if(ThoigianMoMayLocNuoc > 3600)
			{
				coDangBaoVe = true;
				stepLocNuocBinhThuong = 2;
			}
		} else if (stepLocNuocBinhThuong == 2)
		{
			ThoigianMoMayLocNuoc = 0;
			ThoigianTatMayLocNuoc += 1;
			if(ThoigianTatMayLocNuoc > 1800)
			{
				coDangBaoVe = false;
				stepLocNuocBinhThuong = 3;
			}
		} else if (stepLocNuocBinhThuong == 3)
		{
			ThoigianTatMayLocNuoc = 0;
			ThoigianMoMayLocNuoc += 1;
			if(ThoigianMoMayLocNuoc > 3600)
			{
				coDangBaoVe = true;
				stepLocNuocBinhThuong = 4;
			}
		} else if (stepLocNuocBinhThuong == 4)
		{
			ThoigianMoMayLocNuoc = 0;
			ThoigianTatMayLocNuoc += 1;
			if(ThoigianTatMayLocNuoc > 1800)
			{
				stepLocNuocBinhThuong = 5;
				coDangBaoVe = false;
			}
		} else if (stepLocNuocBinhThuong == 5)
		{
			ThoigianMoMayLocNuoc += 1;
			ThoigianTatMayLocNuoc = 0;
			if(ThoigianMoMayLocNuoc > 3600)
			{
				stepLocNuocBinhThuong = 6;
				coDangBaoVe = true;
			}
		}
	}


}

/* Read all ADC Function*/
// void Read_All_ADC(void)
// {
// 	/* Power on ADC module */
// 	ADC_POWER_ON(ADC);

// 	/* Set the ADC operation mode as continuous scan, input mode as single-end and enable the analog input channel 0, 1, 2 and 3 */
// 	ADC_Open(ADC, ADC_ADCR_DIFFEN_SINGLE_END, ADC_ADCR_ADMD_CONTINUOUS, 0xE3);

// 	/* clear the A/D interrupt flag for safe */
// 	ADC_CLR_INT_FLAG(ADC, ADC_ADF_INT);

// 	/* start A/D conversion */
// 	ADC_START_CONV(ADC);
// 	/* Wait conversion done */
// 	while(!ADC_GET_INT_FLAG(ADC, ADC_ADF_INT));
// 	/* Stop A/D conversion */
// 	ADC_STOP_CONV(ADC);
// 	//i32ConversionData = ADC_GET_CONVERSION_DATA(ADC, u32ChannelCount);
// 	adc_array[0] = ADC_GET_CONVERSION_DATA(ADC,1);
// 	adc_array[1] = ADC_GET_CONVERSION_DATA(ADC,0);
// 	adc_array[2] = ADC_GET_CONVERSION_DATA(ADC,5);
// 	adc_array[3] = ADC_GET_CONVERSION_DATA(ADC,6);
// 	adc_array[4] = ADC_GET_CONVERSION_DATA(ADC,7);
// 		/* clear the A/D interrupt flag for safe */
// 	ADC_CLR_INT_FLAG(ADC, ADC_ADF_INT);
// }

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
	static uint8_t step = 0;
}

void SucRuaLoiHai(void)
{
	switch(StepSucRuaLoiHai)
	{
		case 1:
		{
			Van1 = off;
			Van2 = off;
			Van3 = off;
			VanDP = off;
			Van5 = on;
			Motor_220V = on;
			Van6 = on;
			Van4 = on;
			KiemTraDongDien();
			kiemTraDienAp();
			break;
		}
		case 2:
		{
			Van5 = on;
			Motor_220V = on;
			Van6 = off;
			Van1 = off;
			Van2 = on;
			Van3 = off;
			Van4 = on;
			VanDP = off;
			KiemTraDongDien();
			kiemTraDienAp();
			break;
		}
		case 3:
		{
			flagSucRuaLoiHai = false;
			StepSucRuaLoiHai = 1;
			LuuluongSucRuaLoi2 =0;
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
	LuuLuong = (Xung_CB_LL / HesoSanXuat) * HesoCalib;
	LuuluongSucRuaLoi2 = LuuLuong /60 + LuuluongSucRuaLoi2 ;
	Xung_CB_LL = 0;
	printf("tan so luu luong : %d", TanSoLuuLuong);
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
        //PWM_PwmIRQHandler();
    }
}


void KiemTraSucRua(void)
{
	if(LuuluongSucRuaLoi2 > 100)
	{
		flagSucRuaLoiHai = true;
	}
}

void kiemTraDienAp(void)
{
	
}
void KiemTraDongDien(void)
{
	
}