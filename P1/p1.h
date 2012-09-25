#include "lpc_types.h"

void p1_char(uint8_t c);

#define P1_SMID  				"/ISk5\\"    		// Smart Meter ID. Used to detect start of telegram.
#define P1PU1 					"1-0:1.8.1"     // total power usage normal tariff
#define P1PU2 					"1-0:1.8.2"			// total power usage low tariff
#define P1PD1 					"1-0:2.8.1"			// total delivered power normal tariff
#define P1PD2 					"1-0:2.8.2"			// total delivered power low tariff
#define P1TIP           "0-0:96.14.0"   // tariff indicator power
#define P1PUC 					"1-0:1.7.0"			// current power usage
#define P1PDC 					"1-0:2.7.0"			// current power delivery
#define P1GTS  					"0-1:24.3.0"		// timestamp gas usage sample

#define TPF							"/sensor/P1"						// MQTT topic prefix. temporary.


#define LED6_ON         GPIOSetValue(1, 18, 1)
#define LED6_OFF        GPIOSetValue(1, 18, 0)
#define LED7_ON         GPIOSetValue(1, 19, 1)
#define LED7_OFF        GPIOSetValue(1, 19, 0)
#define LED8_ON         GPIOSetValue(1, 24, 1)
#define LED8_OFF        GPIOSetValue(1, 24, 0)
#define LED9_ON         GPIOSetValue(1, 25, 1)
#define LED9_OFF        GPIOSetValue(1, 25, 0)
