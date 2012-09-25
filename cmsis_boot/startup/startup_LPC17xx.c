/**
 ******************************************************************************
 * @file      startup_lpc17xx.c
 * @author    Coocox
 * @version   V1.1
 * @date      02/18/2011
 * @brief     LPC17XX Devices Startup code.
 *            This module performs:
 *                - Set the initial SP
 *                - Set the vector table entries with the exceptions ISR address
 *                - Initialize data and bss
 *                - Setup the microcontroller system.
 *                - Call the application's entry point.
 *            After Reset the Cortex-M3 processor is in Thread mode,
 *            priority is Privileged, and the Stack is set to Main.
 *******************************************************************************
 */
 

/*----------Stack Configuration-----------------------------------------------*/  
#define STACK_SIZE       0x00000100      /*!< Stack size (in Words)           */
__attribute__ ((section(".co_stack")))
unsigned long pulStack[STACK_SIZE];      


/*----------Macro definition--------------------------------------------------*/  
#define WEAK __attribute__ ((weak))           


/*----------Declaration of the default fault handlers-------------------------*/  
/* System exception vector handler */
void WEAK  Reset_Handler(void);   
void WEAK  NMI_Handler(void);       
void WEAK  HardFault_Handler(void); 
void WEAK  MemManage_Handler(void); 
void WEAK  BusFault_Handler(void);  
void WEAK  UsageFault_Handler(void);
void WEAK  SVC_Handler(void);       
void WEAK  DebugMon_Handler(void);  
void WEAK  PendSV_Handler(void);    
void WEAK  SysTick_Handler(void);   
void WEAK  WDT_IRQHandler(void);     
void WEAK  TIMER0_IRQHandler(void);  
void WEAK  TIMER1_IRQHandler(void);  
void WEAK  TIMER2_IRQHandler(void);  
void WEAK  TIMER3_IRQHandler(void);  
void WEAK  UART0_IRQHandler(void);   
void WEAK  UART1_IRQHandler(void);   
void WEAK  UART2_IRQHandler(void);   
void WEAK  UART3_IRQHandler(void);   
void WEAK  PWM1_IRQHandler(void);    
void WEAK  I2C0_IRQHandler(void);       
void WEAK  I2C1_IRQHandler(void);       
void WEAK  I2C2_IRQHandler(void);       
void WEAK  SPI_IRQHandler(void);        
void WEAK  SSP0_IRQHandler(void);       
void WEAK  SSP1_IRQHandler(void);       
void WEAK  PLL0_IRQHandler(void);    
void WEAK  RTC_IRQHandler(void);     
void WEAK  EINT0_IRQHandler(void);   
void WEAK  EINT1_IRQHandler(void);   
void WEAK  EINT2_IRQHandler(void);   
void WEAK  EINT3_IRQHandler(void);   
void WEAK  ADC_IRQHandler(void);     
void WEAK  BOD_IRQHandler(void);     
void WEAK  USB_IRQHandler(void);     
void WEAK  CAN_IRQHandler(void);     
void WEAK  DMA_IRQHandler(void);     
void WEAK  I2S_IRQHandler(void);     
void WEAK  ENET_IRQHandler(void);    
void WEAK  RIT_IRQHandler(void);     
void WEAK  MCPWM_IRQHandler(void);   
void WEAK  QEI_IRQHandler(void);     
void WEAK  PLL1_IRQHandler(void);    
void WEAK  USBActivity_IRQHandler(void);
void WEAK  CANActivity_IRQHandler(void);

/*----------Symbols defined in linker script----------------------------------*/  
extern unsigned long _sidata;    /*!< Start address for the initialization 
                                      values of the .data section.            */
extern unsigned long _sdata;     /*!< Start address for the .data section     */    
extern unsigned long _edata;     /*!< End address for the .data section       */    
extern unsigned long _sbss;      /*!< Start address for the .bss section      */
extern unsigned long _ebss;      /*!< End address for the .bss section        */      
extern void _eram;               /*!< End address for ram                     */


/*----------Function prototypes-----------------------------------------------*/  
extern int main(void);           /*!< The entry point for the application.    */
extern void SystemInit(void);    /*!< Setup the microcontroller system(CMSIS) */
void Default_Reset_Handler(void);   /*!< Default reset handler                */
static void Default_Handler(void);  /*!< Default exception handler            */


/**
  *@brief The minimal vector table for a Cortex M3.  Note that the proper constructs
  *       must be placed on this to ensure that it ends up at physical address
  *       0x00000000.  
  */
__attribute__ ((section(".isr_vector")))
void (* const g_pfnVectors[])(void) =
{	
  /*----------Core Exceptions------------------------------------------------ */
  (void *)&pulStack[STACK_SIZE-1],     /*!< The initial stack pointer         */
  Reset_Handler,             /*!< Reset Handler                               */
  NMI_Handler,               /*!< NMI Handler                                 */
  HardFault_Handler,         /*!< Hard Fault Handler                          */
  MemManage_Handler,         /*!< MPU Fault Handler                           */
  BusFault_Handler,          /*!< Bus Fault Handler                           */
  UsageFault_Handler,        /*!< Usage Fault Handler                         */
  0,0,0,0,                   /*!< Reserved                                    */
  SVC_Handler,               /*!< SVCall Handler                              */
  DebugMon_Handler,          /*!< Debug Monitor Handler                       */
  0,                         /*!< Reserved                                    */
  PendSV_Handler,            /*!< PendSV Handler                              */
  SysTick_Handler,           /*!< SysTick Handler                             */
  
  /*----------External Exceptions---------------------------------------------*/
  WDT_IRQHandler,            /*!<  0: Watchdog Timer                          */
  TIMER0_IRQHandler,         /*!<  1: Timer0                                  */
  TIMER1_IRQHandler,         /*!<  2: Timer1                                  */
  TIMER2_IRQHandler,         /*!<  3: Timer2                                  */
  TIMER3_IRQHandler,         /*!<  4: Timer3                                  */
  UART0_IRQHandler,          /*!<  5: UART0                                   */
  UART1_IRQHandler,          /*!<  6: UART1                                   */
  UART2_IRQHandler,          /*!<  7: UART2                                   */
  UART3_IRQHandler,          /*!<  8: UART3                                   */
  PWM1_IRQHandler,           /*!<  9: PWM1                                    */
  I2C0_IRQHandler,           /*!< 10: I2C0                                    */
  I2C1_IRQHandler,           /*!< 11: I2C1                                    */
  I2C2_IRQHandler,           /*!< 12: I2C2                                    */
  SPI_IRQHandler,            /*!< 13: SPI                                     */
  SSP0_IRQHandler,           /*!< 14: SSP0                                    */
  SSP1_IRQHandler,           /*!< 15: SSP1                                    */
  PLL0_IRQHandler,           /*!< 16: PLL0 (Main PLL)                         */
  RTC_IRQHandler,            /*!< 17: Real Time Clock                         */
  EINT0_IRQHandler,          /*!< 18: External Interrupt 0                    */
  EINT1_IRQHandler,          /*!< 19: External Interrupt 1                    */
  EINT2_IRQHandler,          /*!< 20: External Interrupt 2                    */
  EINT3_IRQHandler,          /*!< 21: External Interrupt 3                    */
  ADC_IRQHandler,            /*!< 22: A/D Converter                           */
  BOD_IRQHandler,            /*!< 23: Brown Out Detect                        */
  USB_IRQHandler,            /*!< 24: USB                                     */
  CAN_IRQHandler,            /*!< 25: CAN                                     */
  DMA_IRQHandler,            /*!< 26: GP DMA                                  */
  I2S_IRQHandler,            /*!< 27: I2S                                     */
  ENET_IRQHandler,           /*!< 28: Ethernet                                */
  RIT_IRQHandler,            /*!< 29: Repetitive Interrupt Timer              */
  MCPWM_IRQHandler,          /*!< 30: Motor Control PWM                       */
  QEI_IRQHandler,            /*!< 31: Quadrature Encoder Interface            */
  PLL1_IRQHandler,           /*!< 32: PLL1 (USB PLL)                          */
  USBActivity_IRQHandler,    /*!< 33: USB Activity                            */
  CANActivity_IRQHandler,    /*!< 34: CAN Activity                            */
};


/**
  * @brief  This is the code that gets called when the processor first
  *         starts execution following a reset event. Only the absolutely
  *         necessary set is performed, after which the application
  *         supplied main() routine is called. 
  * @param  None
  * @retval None
  */
void Default_Reset_Handler(void)
{
  /* Initialize data and bss */
  unsigned long *pulSrc, *pulDest;

  /* Copy the data segment initializers from flash to SRAM */
  pulSrc = &_sidata;

  for(pulDest = &_sdata; pulDest < &_edata; )
  {
    *(pulDest++) = *(pulSrc++);
  }
  
  /* Zero fill the bss segment.  This is done with inline assembly since this
     will clear the value of pulDest if it is not kept in a register. */
  __asm("  ldr     r0, =_sbss\n"
        "  ldr     r1, =_ebss\n"
        "  mov     r2, #0\n"
        "  .thumb_func\n"
        "zero_loop:\n"
        "    cmp     r0, r1\n"
        "    it      lt\n"
        "    strlt   r2, [r0], #4\n"
        "    blt     zero_loop");

  /* Setup the microcontroller system. */
  SystemInit();
	
  /* Call the application's entry point.*/
  main();
}


/**
  *@brief Provide weak aliases for each Exception handler to the Default_Handler. 
  *       As they are weak aliases, any function with the same name will override 
  *       this definition.
  */
#pragma weak Reset_Handler = Default_Reset_Handler
#pragma weak NMI_Handler = Default_Handler     
#pragma weak HardFault_Handler = Default_Handler     
#pragma weak MemManage_Handler = Default_Handler     
#pragma weak BusFault_Handler = Default_Handler      
#pragma weak UsageFault_Handler = Default_Handler    
#pragma weak SVC_Handler = Default_Handler           
#pragma weak DebugMon_Handler = Default_Handler      
#pragma weak PendSV_Handler = Default_Handler        
#pragma weak SysTick_Handler = Default_Handler       
#pragma weak WDT_IRQHandler = Default_Handler        
#pragma weak TIMER0_IRQHandler = Default_Handler     
#pragma weak TIMER1_IRQHandler = Default_Handler     
#pragma weak TIMER2_IRQHandler = Default_Handler     
#pragma weak TIMER3_IRQHandler = Default_Handler     
#pragma weak UART0_IRQHandler = Default_Handler      
#pragma weak UART1_IRQHandler = Default_Handler      
#pragma weak UART2_IRQHandler = Default_Handler      
#pragma weak UART3_IRQHandler = Default_Handler      
#pragma weak PWM1_IRQHandler = Default_Handler       
#pragma weak I2C0_IRQHandler = Default_Handler       
#pragma weak I2C1_IRQHandler = Default_Handler       
#pragma weak I2C2_IRQHandler = Default_Handler       
#pragma weak SPI_IRQHandler = Default_Handler        
#pragma weak SSP0_IRQHandler = Default_Handler       
#pragma weak SSP1_IRQHandler = Default_Handler       
#pragma weak PLL0_IRQHandler = Default_Handler       
#pragma weak RTC_IRQHandler = Default_Handler        
#pragma weak EINT0_IRQHandler = Default_Handler      
#pragma weak EINT1_IRQHandler = Default_Handler      
#pragma weak EINT2_IRQHandler = Default_Handler      
#pragma weak EINT3_IRQHandler = Default_Handler      
#pragma weak ADC_IRQHandler = Default_Handler        
#pragma weak BOD_IRQHandler = Default_Handler        
#pragma weak USB_IRQHandler = Default_Handler        
#pragma weak CAN_IRQHandler = Default_Handler        
#pragma weak DMA_IRQHandler = Default_Handler        
#pragma weak I2S_IRQHandler = Default_Handler        
#pragma weak ENET_IRQHandler = Default_Handler       
#pragma weak RIT_IRQHandler = Default_Handler        
#pragma weak MCPWM_IRQHandler = Default_Handler      
#pragma weak QEI_IRQHandler = Default_Handler        
#pragma weak PLL1_IRQHandler = Default_Handler      
#pragma weak USBActivity_IRQHandler = Default_Handler        
#pragma weak CANActivity_IRQHandler = Default_Handler        


/**
  * @brief  This is the code that gets called when the processor receives an 
  *         unexpected interrupt.  This simply enters an infinite loop, 
  *         preserving the system state for examination by a debugger.
  * @param  None
  * @retval None  
  */
static void Default_Handler(void) 
{
	/* Go into an infinite loop. */
	while (1) 
	{
	}
}

/*********************** (C) COPYRIGHT 2009 Coocox ************END OF FILE*****/
