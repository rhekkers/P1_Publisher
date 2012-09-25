#ifndef __MQTT_H__
#define __MQTT_H__

#include <string.h>

// MQTT_MAX_PACKET_SIZE : Maximum packet size
#define MQTT_MAX_PACKET_SIZE 128
#define MQTT_MAX_PAYLOAD_SIZE 100
#define MQTT_NUMLINES 20

// MQTT_KEEPALIVE : keepAlive interval in Seconds
// TODO: use this for ping timeout etc.
#define MQTT_KEEPALIVE 30
#define MQTT_PROTOCOLVERSION 3
#define MQTT_CLIENTID "smartmeter"

typedef enum {
	RESERVED = 0,
  MQTT_CONNECT,
  MQTT_CONNACK,
  MQTT_PUBLISH,
  MQTT_PUBACK,
  MQTT_PUBREC,
  MQTT_PUBREL,
  MQTT_PUBCOMP,
  MQTT_SUBSCRIBE,
  MQTT_SUBACK,
  MQTT_UNSUBSCRIBE,
  MQTT_UNSUBACK,
  MQTT_PINGREQ,
  MQTT_PINGRESP,
  MQTT_DISCONNECT
} mqtt_msg_t;

struct mqtt_pubmessage
{
	uint8_t len;
  char msg[MQTT_MAX_PACKET_SIZE];
};

struct mqtt_state {
  uint8_t connstate;
  uint8_t numsent;
  uint16_t msgid;

  char buf[MQTT_MAX_PACKET_SIZE];
  char len;
  struct mqtt_pubmessage *msgs[MQTT_NUMLINES];
};

typedef struct mqtt_state uip_tcp_appstate_t;

void mqtt_init(void);
void mqtt_appcall(void);
uint8_t mqtt_connect(char *clientid, char* user, char* pass, char* willTopic, uint8_t willQoS, uint8_t willRetain, char* willMessage);
void mqtt_publish(char *topic, char *payload, size_t pllen, uint8_t retained);
void mqtt_checkconn(void);
void mqtt_ping(void);

#ifndef UIP_APPCALL
#define UIP_APPCALL     mqtt_appcall
#endif

#endif /* __MQTT_H__ */
