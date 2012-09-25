
#include "lpc17xx_systick.h"
#include "lpc_types.h"
#include "LPC17xx.h"

volatile UNS_32 systick_counter = 0;

void SysTick_Handler (void) {

  ++systick_counter;

}

void init_timer ( UNS_32 TimerInterval ) 
{
	systick_counter = 0;
				   
	SysTick_Config(SystemCoreClock/TimerInterval);               /* Generate interrupt every 10 ms */
}

/******************************************************************************
**                            End Of File
******************************************************************************/
