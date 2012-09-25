
extern void GPIOSetDir( uint32_t portNum, uint32_t bitPosi, uint32_t dir );
extern void GPIOSetValue( uint32_t portNum, uint32_t bitPosi, uint32_t bitVal );
extern void GPIOSetPull(uint32_t portNum, uint32_t bitPosi, uint32_t dir);
extern uint32_t GPIOGetValue (uint32_t portNum, uint32_t bitPosi);
void GPIOSetInterrupt (  uint32_t portNum, uint32_t bitPosi, uint32_t dir );
void GPIOClearInterrupt( void );
uint32_t GPIOCheckInterrupts ( uint32_t portNum, uint32_t dir);

#define  NOPULL  	0
#define  PULLUP  	1
#define  PULLDOWN  	2

#define  INPUT 		0
#define  OUTPUT 	1

#define  FALLING	0
#define  RISING  	1

#define  LOW		0
#define  HIGH		1
