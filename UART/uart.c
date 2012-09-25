//*****************************************************************************
//   +--+       
//   | ++----+   
//   +-++    |  
//     |     |  
//   +-+--+  |   
//   | +--+--+  
//   +----+    Copyright (c) 2009 Code Red Technologies Ltd. 
//
// UART example project for RDB1768 development board
//
// Software License Agreement
// 
// The software is owned by Code Red Technologies and/or its suppliers, and is 
// protected under applicable copyright laws.  All rights are reserved.  Any 
// use in violation of the foregoing restrictions may subject the user to criminal 
// sanctions under applicable laws, as well as to civil liability for the breach 
// of the terms and conditions of this license.
// 
// THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
// OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
// USE OF THIS SOFTWARE FOR COMMERCIAL DEVELOPMENT AND/OR EDUCATION IS SUBJECT
// TO A CURRENT END USER LICENSE AGREEMENT (COMMERCIAL OR EDUCATIONAL) WITH
// CODE RED TECHNOLOGIES LTD. 
//
//*****************************************************************************


#include "LPC17xx.h"

// PCUART0
#define PCUART0_POWERON (1 << 3)
#define PCUART1_POWERON (1 << 4)
#define PCUART2_POWERON (1 << 24)
#define PCUART3_POWERON (1 << 25)

#define PCLK_UART0 6
#define PCLK_UART1 8
#define PCLK_UART2 16
#define PCLK_UART3 18
#define PCLK_UART0_MASK (3 << 6)
#define PCLK_UART1_MASK (3 << 8)
#define PCLK_UART2_MASK (3 << 16)
#define PCLK_UART3_MASK (3 << 18)

#define IER_RBR		0x01
#define IER_THRE	0x02
#define IER_RLS		0x04

#define IIR_PEND	0x01
#define IIR_RLS		0x03
#define IIR_RDA		0x02
#define IIR_CTI		0x06
#define IIR_THRE	0x01

#define LSR_RDR		0x01
#define LSR_OE		0x02
#define LSR_PE		0x04
#define LSR_FE		0x08
#define LSR_BI		0x10
#define LSR_THRE	0x20
#define LSR_TEMT	0x40
#define LSR_RXFE	0x80

// ***********************
// Function to set up UART
void UART0_Init(int baudrate)
{
	int pclk;
	unsigned long int Fdiv;

	// PCLK_UART0 is being set to 1/4 of SystemCoreClock
	pclk = SystemCoreClock / 4;	
	
	// Turn on power to UART0
	LPC_SC->PCONP |=  PCUART0_POWERON;
		
	// Turn on UART0 peripheral clock
	LPC_SC->PCLKSEL0 &= ~(PCLK_UART0_MASK);
	LPC_SC->PCLKSEL0 |=  (0 << PCLK_UART0);		// PCLK_periph = CCLK/4
	
	// Set PINSEL0 so that P0.2 = TXD0, P0.3 = RXD0
	LPC_PINCON->PINSEL0 &= ~0xf0;
	LPC_PINCON->PINSEL0 |= ((1 << 4) | (1 << 6));
	
	//	Bit		 Val			Descr.
	// 	1:0			00			5	data bits
	//					01			6
	//					10			7
	//					11			8
	//	2				 0			1 stop bits
	//					 1			2
	//	3				 0			Disable parity
	//					 1			Enable
	//
	// 	5:4			00			Odd
	//					01			Even
	//					10			Forced "1" stick
	//					11			Forced "0" stick

	//LPC_UART0->LCR = 0x83;	// 8 bits, no Parity, 1 Stop bit, DLAB=1
	LPC_UART0->LCR = 0x8A;    // 7 bits, Even Parity, 1 Stop bit, DLAB=1


  Fdiv = ( pclk / 16 ) / baudrate ;	// Set baud rate
  LPC_UART0->DLM = Fdiv / 256;
  LPC_UART0->DLL = Fdiv % 256;

  //LPC_UART0->LCR = 0x03;	// 8 bits, no Parity, 1 Stop bit DLAB = 0
  LPC_UART0->LCR = 0x0A;    // 7 bits, Even Parity, 1 Stop bit, DLAB=1

  LPC_UART0->FCR = 0x07;		// Enable and reset TX and RX FIFO
}

void UART1_Init(int baudrate)
{
	int pclk;
	unsigned long int Fdiv;

	// PCLK_UART0 is being set to 1/4 of SystemCoreClock
	pclk = SystemCoreClock / 4;

	// Turn on power to UART0
	LPC_SC->PCONP |=  PCUART1_POWERON;

	// Turn on UART0 peripheral clock
	LPC_SC->PCLKSEL0 &= ~(PCLK_UART1_MASK);
	LPC_SC->PCLKSEL0 |=  (0 << PCLK_UART1);		// PCLK_periph = CCLK/4

	// Set PINSEL0 so that P0.2 = TXD0, P0.3 = RXD0
	LPC_PINCON->PINSEL0 &= ~0xc0000000;
	LPC_PINCON->PINSEL0 |= (1 << 30);
	LPC_PINCON->PINSEL1 &= ~0xc;
	LPC_PINCON->PINSEL1 |= (1 << 0);

	LPC_UART1->LCR = 0x83;		// 8 bits, no Parity, 1 Stop bit, DLAB=1
    Fdiv = ( pclk / 16 ) / baudrate ;	// Set baud rate
    LPC_UART1->DLM = Fdiv / 256;
    LPC_UART1->DLL = Fdiv % 256;
    LPC_UART1->LCR = 0x03;		// 8 bits, no Parity, 1 Stop bit DLAB = 0
    LPC_UART1->FCR = 0x07;		// Enable and reset TX and RX FIFO
}

void UART2_Init(int baudrate)
{
	int pclk;
	unsigned long int Fdiv;

	// PCLK_UART2 is being set to 1/4 of SystemCoreClock
	pclk = SystemCoreClock / 4;

	// Turn on power to UART2
	LPC_SC->PCONP |=  PCUART0_POWERON;

	// Turn on UART2 peripheral clock
	LPC_SC->PCLKSEL1 &= ~(PCLK_UART2_MASK);
	LPC_SC->PCLKSEL1 |=  (0 << PCLK_UART2);		// PCLK_periph = CCLK/4

	// Set PINSEL0 so that P0.10 = TXD2, P0.11 = RXD2
	LPC_PINCON->PINSEL0 &= ~0xf00000;
	LPC_PINCON->PINSEL0 |= ((1 << 20) | (1 << 22));

	LPC_UART2->LCR = 0x83;		// 8 bits, no Parity, 1 Stop bit, DLAB=1
    Fdiv = ( pclk / 16 ) / baudrate ;	// Set baud rate
    LPC_UART2->DLM = Fdiv / 256;
    LPC_UART2->DLL = Fdiv % 256;
    LPC_UART2->LCR = 0x03;		// 8 bits, no Parity, 1 Stop bit DLAB = 0
    LPC_UART2->FCR = 0x07;		// Enable and reset TX and RX FIFO
}

void UART3_Init(int baudrate)
{
	int pclk;
	unsigned long int Fdiv;

	// PCLK_UART2 is being set to 1/4 of SystemCoreClock
	pclk = SystemCoreClock / 4;

	// Turn on power to UART2
	LPC_SC->PCONP |=  PCUART3_POWERON;

	// Turn on UART2 peripheral clock
	LPC_SC->PCLKSEL1 &= ~(PCLK_UART3_MASK);
	LPC_SC->PCLKSEL1 |=  (0 << PCLK_UART3);		// PCLK_periph = CCLK/4

	// Set PINSEL0 so that P0.0 = TXD3, P0.1 = RXD3
	LPC_PINCON->PINSEL0 &= ~0xf;
	LPC_PINCON->PINSEL0 |= ((1 << 1) | (1 << 3));

	LPC_UART3->LCR = 0x83;		// 8 bits, no Parity, 1 Stop bit, DLAB=1
    Fdiv = ( pclk / 16 ) / baudrate ;	// Set baud rate
    LPC_UART3->DLM = Fdiv / 256;
    LPC_UART3->DLL = Fdiv % 256;
    LPC_UART3->LCR = 0x03;		// 8 bits, no Parity, 1 Stop bit DLAB = 0
    LPC_UART3->FCR = 0x07;		// Enable and reset TX and RX FIFO
}
// ***********************
// Function to send character over UART
void UART0_Sendchar(char c)
{
	while( (LPC_UART0->LSR & LSR_THRE) == 0 );	// Block until tx empty
	
	LPC_UART0->THR = c;
}

void UART1_Sendchar(char c)
{
	while( (LPC_UART1->LSR & LSR_THRE) == 0 );	// Block until tx empty

	LPC_UART1->THR = c;
}

void UART2_Sendchar(char c)
{
	while( (LPC_UART2->LSR & LSR_THRE) == 0 );	// Block until tx empty

	LPC_UART2->THR = c;
}

void UART3_Sendchar(char c)
{
	while( (LPC_UART3->LSR & LSR_THRE) == 0 );	// Block until tx empty

	LPC_UART3->THR = c;
}

// ***********************
// Function to get character from UART
char UART0_Getchar()
{
	char c;
	while( (LPC_UART0->LSR & LSR_RDR) == 0 );  // Nothing received so just block 	
	c = LPC_UART0->RBR; // Read Receiver buffer register
	return c;
}

char UART1_Getchar()
{
	char c;
	while( (LPC_UART1->LSR & LSR_RDR) == 0 );  // Nothing received so just block
	c = LPC_UART1->RBR; // Read Receiver buffer register
	return c;
}

char UART2_Getchar()
{
	char c;
	while( (LPC_UART2->LSR & LSR_RDR) == 0 );  // Nothing received so just block
	c = LPC_UART2->RBR; // Read Receiver buffer register
	return c;
}

char UART3_Getchar()
{
	char c;
	while( (LPC_UART3->LSR & LSR_RDR) == 0 );  // Nothing received so just block
	c = LPC_UART3->RBR; // Read Receiver buffer register
	return c;
}

// ***********************
// Function to determine if data has been received
int UART0_Available()
{
	return ( (LPC_UART0->LSR & LSR_RDR) != 0 );  // Received?
}

int UART1_Available()
{
	return ( (LPC_UART1->LSR & LSR_RDR) != 0 );  // Received?
}

int UART2_Available()
{
	return ( (LPC_UART2->LSR & LSR_RDR) != 0 );  // Received?
}

int UART3_Available()
{
	return ( (LPC_UART3->LSR & LSR_RDR) != 0 );  // Received?
}

// ***********************
// Function to prints the string out over the UART
void UART0_PrintString(char *pcString)
{
	int i = 0;
	// loop through until reach string's zero terminator
	while (pcString[i] != 0)
	{
		UART0_Sendchar(pcString[i]); // print each character
		i++;
	}
}

void UART1_PrintString(char *pcString)
{
	int i = 0;
	// loop through until reach string's zero terminator
	while (pcString[i] != 0)
	{
		UART1_Sendchar(pcString[i]); // print each character
		i++;
	}
}

void UART2_PrintString(char *pcString)
{
	int i = 0;
	// loop through until reach string's zero terminator
	while (pcString[i] != 0)
	{
		UART2_Sendchar(pcString[i]); // print each character
		i++;
	}
}

void UART3_PrintString(char *pcString)
{
	int i = 0;
	// loop through until reach string's zero terminator
	while (pcString[i] != 0)
	{
		UART3_Sendchar(pcString[i]); // print each character
		i++;
	}
}
