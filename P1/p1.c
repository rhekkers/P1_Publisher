#include "p1.h"
#include "mqtt.h"
#include <string.h>

#define P1_LINELEN 80
typedef struct {
	uint8_t exclmarkfound;
	uint8_t linecount;
	uint8_t len;
  char line[P1_LINELEN];
} p1_state;

p1_state p1data;

typedef enum { ID, STD, LINE17, LINE18, EXCLMARK } MatchType;
typedef struct {
	MatchType type;
  char* key;
  char* topic;
  int start;
  int width;
} Match;

Match matchlist[] = {
  {ID, "/ISk5\\", "", 0, 0},
  {STD, "1-0:1.8.1", TPF"/powerusage1", 10, 9},
  {STD, "1-0:1.8.2", TPF"/powerusage2", 10, 9},
  {STD, "1-0:2.8.1", TPF"/powerdeliv1", 10, 9},
  {STD, "1-0:2.8.2", TPF"/powerdeliv2", 10, 9},
  {STD, "0-0:96.14.0", TPF"/tariff", 12, 4},
  {STD, "1-0:1.7.0", TPF"/powerusagec", 10, 7},
  {STD, "1-0:2.7.0", TPF"/powerdelivc", 10, 7},
  {LINE17, "1-0:2.7.0", TPF"/gastimestamp", 11, 12},
  {LINE18, "(", TPF"/gasusage", 1, 9},
  {EXCLMARK,"!","", 0, 0}
};

//---------------------------------------------------------------------------
void send(char *str1, char *str2)
{
	mqtt_publish(str1, str2, strlen(str2), 1 /* (retain) */);
}
//---------------------------------------------------------------------------
void matchline()
{
	uint8_t i;
	uint8_t found=0;
	Match t;
	char value[13]="";

	for(i=0;(i<sizeof(matchlist)/sizeof(Match))&(!found);i++){
		t = matchlist[i];
//		uint8_t keymatch = (strncmp(t.key, p1data.line, strlen(t.key)) == 0);
//		if(keymatch){
			switch(t.type){
			case ID: if(strncmp(t.key, p1data.line, strlen(t.key)) == 0){
				p1data.linecount=1;
				break;
			}
			case STD: if(strncmp(t.key, p1data.line, strlen(t.key)) == 0){
				found=1;
				break;
			}
			case LINE17:if(strncmp(t.key, p1data.line, strlen(t.key)) == 0){
				p1data.linecount = 17;
				found=1;
				break;
			}
			case LINE18:if((p1data.linecount == 18)&&(strncmp(t.key, p1data.line, strlen(t.key)) == 0)){
				found=1;
				break;
			}
			case EXCLMARK:if(strncmp(t.key, p1data.line, strlen(t.key)) == 0){
				p1data.exclmarkfound=1;
				break;
			}
			} //switch

			if((found)&&(p1data.exclmarkfound)){
				strncpy(value, p1data.line+t.start, t.width);
				send(t.topic, value);
			//}
	}
	}
}
//---------------------------------------------------------------------------
// character received will be stored in linebuffer, which will be
// processed when a line-feed (0x0a) is encountered.
void p1_char(uint8_t c)
{
  if(c == 0x0d) return;

  p1data.line[p1data.len] = c;
  if(p1data.line[p1data.len] == 0x0a || p1data.len == sizeof(p1data.line) - 1)
  {
    // discard newline, close string, parse line and clear it.
    if(p1data.len > 0) p1data.line[p1data.len] = 0;
    p1data.linecount++;
    matchline();
    p1data.len = 0;
  }
  else
  {
    ++p1data.len;
  }
}
