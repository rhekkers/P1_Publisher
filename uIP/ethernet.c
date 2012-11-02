#include "LPC17xx.h"
#include "lpc_types.h"
#include "ethernet.h"
#include "uiptimer.h"
#include "uip.h"
#include "uip_arp.h"
#include "tapdev.h"
#include "timer.h"
#include "leds.h"
#include "mqtt.h"

	unsigned int i;
	uip_ipaddr_t ipaddr;	/* local IP address */
	//uip_ipaddr_t ipaddrs;	/* local IP address */
	struct timer periodic_timer, arp_timer, alwaysconn_timer;
	char ipstring [20];

void EthernetInit (void)
{

	SystemInit();                                      /* setup core clocks */


	// clock init
	clock_init();

	// two timers for tcp/ip
	timer_set(&periodic_timer, CLOCK_SECOND/8 ); /* 0.25s */
	timer_set(&arp_timer, CLOCK_SECOND * 10);	/* 10s */
	timer_set(&alwaysconn_timer, CLOCK_SECOND * 15);	/* 15s */

	// ethernet init
	tapdev_init();

	// Initialize the uIP TCP/IP stack.
	uip_init();

	uip_ipaddr(ipaddr, MYIP_1,MYIP_2,MYIP_3,MYIP_4);
	uip_sethostaddr(ipaddr);	/* host IP address */
	uip_ipaddr(ipaddr, MYIP_1,MYIP_2,MYIP_3,1);
	uip_setdraddr(ipaddr);	/* router IP address */
	uip_ipaddr(&ipaddr, 255,255,255,0);
	uip_setnetmask(ipaddr);	/* mask */

	mqtt_init();
}

void EthernetHandle (void)
{
	uip_len = tapdev_read(uip_buf);
	if(uip_len > 0)		/* received packet */
	{
  	if(BUF->type == htons(UIP_ETHTYPE_IP))	/* IP packet */
  	{
    	uip_arp_ipin();
    	uip_input();

    	/* If the above function invocation resulted in data that
     		should be sent out on the network, the global variable
     		uip_len is set to a value > 0.
     	*/

    	if(uip_len > 0)
    	{
    		uip_arp_out();
     		tapdev_send(uip_buf,uip_len);
    	}
  	}
    else if(BUF->type == htons(UIP_ETHTYPE_ARP))	/*ARP packet */
    {
     	uip_arp_arpin();
	   	/* If the above function invocation resulted in data that
	     	should be sent out on the network, the global variable
	     	uip_len is set to a value > 0. */
	   	if(uip_len > 0)
     	{
	     	tapdev_send(uip_buf,uip_len);	/* ARP ack*/
	   	}
    }
	}
	else if(timer_expired(&periodic_timer))	/* no packet but periodic_timer time out (0.5s)*/
	{
  	timer_reset(&periodic_timer);

  	for(i = 0; i < UIP_CONNS; i++)
  	{
  		uip_periodic(i);
	     /* If the above function invocation resulted in data that
	           should be sent out on the network, the global variable
	           uip_len is set to a value > 0. */
	    if(uip_len > 0)
	    {
	      uip_arp_out();
	      tapdev_send(uip_buf,uip_len);
	    }
  	}
    /* Call the ARP timer function every 10 seconds. */
		if(timer_expired(&arp_timer))
		{
			timer_reset(&arp_timer);
			uip_arp_timer();
		}
	}
	else if(timer_expired(&alwaysconn_timer))
	{
  	timer_reset(&alwaysconn_timer);

  	if(!uip_conn_active(0))
  	{
  		CONNLED_OFF;
  		mqtt_init();
  	}
	}
}
