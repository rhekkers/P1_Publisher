#include "lpc17xx.h"
#include "lpc_types.h"
#include "GPIO.h"
static LPC_GPIO_TypeDef (* const LPC_GPIO[5]) = { LPC_GPIO0, LPC_GPIO1, LPC_GPIO2, LPC_GPIO3, LPC_GPIO4  };


void GPIOSetValue( uint32_t portNum, uint32_t bitPosi, uint32_t bitVal )
{
  if (bitVal == 0)
  {
	  LPC_GPIO[portNum]->FIOCLR = (1<<bitPosi);
  }
  else if (bitVal >= 1)
  {
	  LPC_GPIO[portNum]->FIOSET = (1<<bitPosi);
  }
}

void GPIOSetDir( uint32_t portNum, uint32_t bitPosi, uint32_t dir )
{
  if(dir)
	LPC_GPIO[portNum]->FIODIR |= 1<<bitPosi;
  else
	LPC_GPIO[portNum]->FIODIR &= ~(1<<bitPosi);
}

void GPIOSetPull( uint32_t portNum, uint32_t bitPosi, uint32_t dir)
{

	if (dir == 0) {								//no Pull
		dir = 10;
	} else if(dir == 1){   						//Pull up
		dir = 00;
	} else if(dir == 2){						//Pull down
		dir = 11;
	}

	switch (portNum)
	{
		case 0:

			if (bitPosi < 16 ) {
				bitPosi = bitPosi * 2;
				LPC_PINCON->PINMODE0 |= dir<<bitPosi;
			} else if (bitPosi > 15){
				bitPosi = bitPosi - 16;
				bitPosi = bitPosi * 2;
				LPC_PINCON->PINMODE1 |= dir<<bitPosi;
			}

		break;

		case 1:

			if (bitPosi < 16 ) {
				bitPosi = bitPosi * 2;
				LPC_PINCON->PINMODE2 |= dir<<bitPosi;
			} else if (bitPosi > 15){
				bitPosi = bitPosi - 16;
				bitPosi = bitPosi * 2;
				LPC_PINCON->PINMODE3 |= dir<<bitPosi;
			}

		break;

		case 2:

			if (bitPosi < 14 ) {
				bitPosi = bitPosi * 2;
				LPC_PINCON->PINMODE4 |= dir<<bitPosi;
			}

		break;

		case 3:

			if (bitPosi == 25){
				LPC_PINCON->PINMODE7 |= dir<<18;
			}else if (bitPosi == 26){
				LPC_PINCON->PINMODE7 |= dir<<20;
			}

		break;

		case 4:
			if (bitPosi == 28){
				LPC_PINCON->PINMODE9 |= dir<<24;
			}else if (bitPosi == 29){
				LPC_PINCON->PINMODE9 |= dir<<26;
			}

	}
}

uint32_t GPIOGetValue (uint32_t portNum, uint32_t bitPosi)
{
    uint32_t val;
    LPC_GPIO[portNum]->FIOMASK = ~(1<<bitPosi);
    val = LPC_GPIO[portNum]->FIOPIN;
    val = val >> bitPosi;
    LPC_GPIO[portNum]->FIOMASK = 0x00000000;
    return val;
}

void GPIOSetInterrupt (  uint32_t portNum, uint32_t bitPosi, uint32_t dir )
{
	//  Dir is 0 for falling edge interrupt and 1 for rising edge interrupt
	if (portNum == 0)
	{
		if (dir == 0)
		{
			LPC_GPIOINT->IO0IntEnF |= (1<<bitPosi);
		}
		else if (dir == 1)
		{
			LPC_GPIOINT->IO0IntEnR |= (1<<bitPosi);
		}
	}
	else if (portNum == 2)
	{
		if (dir == 0)
		{
			LPC_GPIOINT->IO2IntEnF |= (1<<bitPosi);
		}
		else if (dir == 1)
		{
			LPC_GPIOINT->IO2IntEnR |= (1<<bitPosi);
		}
	}

	NVIC_EnableIRQ(EINT3_IRQn);
}

void GPIOClearInterrupt( void )
{
	LPC_GPIOINT->IO0IntClr = 0x7FFF8FFF;
	LPC_GPIOINT->IO2IntClr = 0x3fff;
}

uint32_t GPIOCheckInterrupts ( uint32_t portNum, uint32_t dir)
{
	//  Dir is 0 for falling edge interrupt and 1 for rising edge interrupt
	if (portNum == 0)
	{
		if (dir == 0)
		{
			return LPC_GPIOINT->IO0IntStatF;
		}
		else if (dir == 1)
		{
			return LPC_GPIOINT->IO0IntStatR;
		}
	}
	else if (portNum == 2)
	{
		if (dir == 0)
		{
			return LPC_GPIOINT->IO0IntStatF;
		}
		else if (dir == 1)
		{
			return LPC_GPIOINT->IO0IntStatR;
		}
	}
	return FALSE;
}
