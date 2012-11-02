#include "LPC17xx.h"
#include "lpc_types.h"
#include "uIP/ethernet.h"
#include "UART/uart.h"
#include "p1.h"
#include "timer.h"
#include "GPIO.h"
#include "mqtt.h"
#include "leds.h"

uint32_t toggle = 0;
static volatile int seccount=0;

int main(void)
{
	char incoming;
	EthernetInit();
	UART0_Init(9600);

	TimerInit(0, 1000);
	TimerInit(3, 120000000);
//	delayMs(0,1000);
//	enable_timer(3);

	while(1)
	{
		EthernetHandle();

		if(UART0_Available())
		{
			UARTLED_ON;
			incoming = UART0_Getchar();
			p1_char(incoming);
			UARTLED_OFF;
		}
	}
}

//void TIMER3_IRQHandler (void)
//{
//	TIMER3_interrupt();
//	toggle = ~toggle;
//	GPIOSetValue(1, 19, toggle);  // LED7
//	seccount++;
//	if(seccount>30){
//		mqtt_checkconn();
//		seccount=0;
//	}
//}
