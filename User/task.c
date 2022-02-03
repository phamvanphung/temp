#include <stdio.h>
#include "include_all_lib.h"
#include "timer.h"

/*------------- global variable ------------*/
uint8_t time_array[6] = {0};
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
	DS1307_INIT();
	time_array[3] = 0x15;
	UART_Write(UART1,time_array + 3,1);
	time.s = 0;
	time.m = 49;
	time.h = 13;
	time.dy = 3;
	time.mt = 2;
	time.yr = 21;
	DS1307_set_time();

	TIMER_Delay(TIMER3,5000);
	DS1307_get_time();
	time_array[0] = time.s;
	time_array[1] = time.m;
	time_array[2] = time.h;
	UART_Write(UART1,time_array,3);


}