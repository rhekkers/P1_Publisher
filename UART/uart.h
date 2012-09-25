//*****************************************************************************
//   +--+       
//   | ++----+   
//   +-++    |  
//     |     |  
//   +-+--+  |   
//   | +--+--+  
//   +----+    Copyright (c) 2009 Code Red Technologies Ltd. 
//
// UART example header file
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

#ifndef UART_H_
#define UART_H_

#define UART_PARITY_NONE 		0b000
#define UART_PARITY_EVEN    0b001
#define UART_PARITY_ODD			0b011

// ***********************
// Function to set up UART
void UART0_Init(int baudrate);
void UART1_Init(int baudrate);
void UART2_Init(int baudrate);
void UART3_Init(int baudrate);

// ***********************
// Function to send character over UART
void UART0_Sendchar(char c);
void UART1_Sendchar(char c);
void UART2_Sendchar(char c);
void UART3_Sendchar(char c);

// ***********************
// Function to get character from UART
char UART0_Getchar();
char UART1_Getchar();
char UART2_Getchar();
char UART3_Getchar();
// ***********************
// Function to determine if data has been received

int UART0_Available();
int UART1_Available();
int UART2_Available();
int UART3_Available();


// ***********************
// Function to prints the string out over the UART
void UART0_PrintString(char *pcString);
void UART1_PrintString(char *pcString);
void UART2_PrintString(char *pcString);
void UART3_PrintString(char *pcString);

#endif /*UART_H_*/
