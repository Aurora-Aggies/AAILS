#include <EEPROM.h>

#include "aails.h"
#include "parser.h"
#include <SPI.h>
#include <Ethernet2.h>
//#include <ArduinoJson.h>
#include <avr/pgmspace.h>

boolean conn = false;
boolean roomOn = false;
unsigned long start_time;
byte hour;
byte mac[] = { 0x2C, 0xF7, 0xF1, 0x08, 0x05, 0x4D };
RoomClass mainRoom;
IPAddress server(192, 168, 137, 1);
IPAddress ip(192, 168, 137, 1);
EthernetClient client;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); //Initializes Serial Communications
  while (!Serial){
    ; //Wait for Serial to connect
  }
  Serial.println(freeRam()); //Prints available ram upon startup
  
  if (EEPROM.read(0) == 1){
    roomOn = true;
    int sz = 0;
    hour = EEPROM.read(1);
    EEPROM.get(2,sz); //Read size of JSON string
    EEPROM.get(4,mainRoom);
  }

  //Start Ethernet Connection
  if (Ethernet.begin(mac) == 0) {
    //Serial.println("Failed to configure Ethernet using DHCP");
    // try to congifure using IP address instead of DHCP:
    //Ethernet.begin(mac, ip);
  }
  // give the Ethernet shield a second to initialize:
  delay(1000);
  //Serial.println("connecting...");
  
  if (client.connect(server, 8080)) {
    //Serial.println("connected");
    conn = true;
  } else {
    // if you didn't get a connection to the server:
    //Serial.println("connection failed");
  }

  if (!roomOn) hour = 0;
  start_time = 0;
}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned long end_time = millis();
  unsigned long elapse = end_time - start_time;
  if (elapse >= 10000){
    //Every 10 seconds will represent an hour
    start_time = millis();
    hour += elapse/10000;
    if (hour > 24)
      hour = 1;
    if (roomOn){
      EEPROM.update(1,hour); //attempt to remember hour
      mainRoom.cycle(hour);
    }
  }
  if (conn){
    // Make a HTTP request:
    //Example of different GETS client.println("GET /changed-brightness?r=1 HTTP/1.1");
    boolean change = getBrightChange(client,server);
    if (change) getRoom(client,roomOn, mainRoom);
  }
}
