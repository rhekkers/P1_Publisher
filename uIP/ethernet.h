void EthernetHandle(void);
void EthernetInit (void);

#define MYIP_1	192
#define MYIP_2	168
#define MYIP_3	10
#define MYIP_4	206 //!!
#define BUF ((struct uip_eth_hdr *)&uip_buf[0])

#define HISIP_1	192
#define HISIP_2	168
#define HISIP_3	10
#define HISIP_4	40 //!!
#define HISPORT 1883
