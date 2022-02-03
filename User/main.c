/******************************************************************************
 * @file     main.c
 * @version  V1.00
 * $Revision: 3 $
 * $Date: 14/01/28 11:45a $
 * @brief   Core May Loc Nuoc Co PLC
 *
 * @note
 * Copyright (C) 2014 Nuvoton Technology Corp. All rights reserved.
*****************************************************************************/
#include <stdio.h>
#include "include_all_lib.h"

uint8_t array[10] = {0xff,0x36};
int main()
{
   /* Unlock protected registers */
    SYS_UnlockReg();
	/* Set up CLK System here*/
		SYS_Init();
	/* Init uart_1 clk and map pin*/
		SYS_Uart1();
	/* Lock protected registers */
    SYS_LockReg();
	
	/*UART OPEN*/
	UART_SetLine_Config(UART1, 115200, UART_WORD_LEN_8,UART_PARITY_NONE,UART_STOP_BIT_1);
	UART_Open(UART1,115200);
	UART_Write(UART1,array,1);
	UART_Write(UART1,array +1,1);
	/* Loop */
	while(true){
		
	}
}

/*** (C) COPYRIGHT 2014 Nuvoton Technology Corp. ***/
