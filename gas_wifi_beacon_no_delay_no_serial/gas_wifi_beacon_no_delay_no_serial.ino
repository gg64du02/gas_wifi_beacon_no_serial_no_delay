#include <ESP8266WiFi.h>

extern "C" {
  #include "user_interface.h"
}

byte channel;

#define MAX_SSID 2 /* how much SSIDs we have */
char *ssids[MAX_SSID] = {
      "i   ", 
      "This is a placeholder"
      };

/*
#define MAX_SSID 10 /* how much SSIDs we have
char *ssids[MAX_SSID] = {
      "One", 
      "Two",  
      "Three",
      "Four",
      "Five",
      "Six",
      "Seven",
      "Eight",
      "Nine",
      "Ten"
      };
*/

byte rnd;
byte i;
byte count;


byte wifipkt[128] = { 0x80, 0x00, 0x00, 0x00, 
                /*4*/   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                /*10*/  0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
                /*16*/  0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 
                /*22*/  0xc0, 0x6c, 
                /*24*/  0x83, 0x51, 0xf7, 0x8f, 0x0f, 0x00, 0x00, 0x00, 
                /*32*/  0x64, 0x00, 
                /*34*/  0x01, 0x04, 
                /* SSID */
                /*36*/  0x00};

byte pktsuffix[] = {
                        0x01, 0x08, 0x82, 0x84,
                        0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03, 0x01, 
                        0x04 };                       

void setup() {
  delay(500);
  wifi_set_opmode(STATION_MODE);
  wifi_promiscuous_enable(1); 
  /*
  // Initialize the serial port
  Serial.begin(115200);
  */
}


//added
int mq135 = A0; // smoke sensor is connected with the analog pin A0 
int data = 0; 
int whatever = 0;

void loop() {

    whatever=whatever+1;
    if(whatever>=10000){
      whatever=0;
    }
    data = whatever;
    //delay(100);
    char cstr[16];
    itoa(data, cstr, 10);
    //Serial.println("data:"+String(cstr));
    /*
    Serial.println("ssids[0]:"+(*ssids[0]));
    */
    //strcpy(ssids[1],cstr);
    //strcpy(ssids[0],cstr);
    strcpy(ssids[0]+0,cstr);

    wifipkt[10] = wifipkt[16] = random(256);
    wifipkt[11] = wifipkt[17] = random(256);
    wifipkt[12] = wifipkt[18] = random(256);
    wifipkt[13] = wifipkt[19] = random(256);
    wifipkt[14] = wifipkt[20] = random(256);
    wifipkt[15] = wifipkt[21] = random(256);

    count=37;

    rnd=random(MAX_SSID);
 
    wifipkt[count++]=strlen(ssids[rnd]);
    for (i=0; i<strlen(ssids[rnd]); i++) {
      wifipkt[count++]=ssids[rnd][i];
    }
    
    for (i=0; i<sizeof(pktsuffix); i++) {
       wifipkt[count++]=pktsuffix[i];
    }

    channel = random(1,12); 
    wifi_set_channel(channel);
    wifipkt[count-1] = channel;
    
    wifi_send_pkt_freedom(wifipkt, count, 0);
    wifi_send_pkt_freedom(wifipkt, count, 0);
    wifi_send_pkt_freedom(wifipkt, count, 0);
    delay(1);
}
