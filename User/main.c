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


int main()
{
   /* Unlock protected registers */
    SYS_UnlockReg();
	/* set up CLK System here*/
		SYS_Init();
	/* Lock protected registers */
    SYS_LockReg();
}

/*** (C) COPYRIGHT 2014 Nuvoton Technology Corp. ***/
