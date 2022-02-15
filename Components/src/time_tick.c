#include <stdio.h>
#include "M051Series.h"
#include "time_tick.h"
#include "task.h"


void TimerTickInit(void)
{
    TIMER_Open(TIMER0, TIMER_PERIODIC_MODE, 1); // 1 interrup /.sec => 1s.
    TIMER_EnableInt(TIMER0);
    NVIC_EnableIRQ(TMR0_IRQn);
		TIMER_Start(TIMER0);
}

void TMR0_IRQHandler(void)
{
    if(TIMER_GetIntFlag(TIMER0) == 1)
    {
      /* Clear Timer0 time-out interrupt flag */
      TIMER_ClearIntFlag(TIMER0);
			// Do some thing
      Doc_CB_LL();
      ThoiGianChuTrinh();
    }
}
