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
int hour;
byte mac[] = { 0x2C, 0xF7, 0xF1, 0x08, 0x05, 0x4D };
RoomClass mainRoom;
IPAddress server(192, 168, 137, 1);
IPAddress ip(192, 168, 137, 1);
EthernetClient client;

void setup() {
  // put your setup code here, to run once:
  hour = 0;
  start_time = 0;
  Serial.begin(9600); //Initializes Serial Communications
  while (!Serial){
    ; //Wait for Serial to connect
  }
  Serial.println(freeRam()); //Prints available ram upon startup
  
  if (EEPROM.read(0) == 1){
    roomOn = true;
    int sz = 0; 
    EEPROM.get(1,sz); //Read size of JSON string
    String m = "";
    for (int i = 3; i < sz; i++){
      char c = EEPROM.read(i);
      m += c;
    }
    //mainRoom = computeRoom(m);
  }

  //Start Ethernet Connection
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // try to congifure using IP address instead of DHCP:
    Ethernet.begin(mac, ip);
  }
  // give the Ethernet shield a second to initialize:
  delay(1000);
  Serial.println("connecting...");
  
  if (client.connect(server, 8080)) {
    //Lets set a universal booelean here
    Serial.println("connected");
    conn = true;
  } else {
    // if you didn't get a connection to the server:
    Serial.println("connection failed");
    return (0);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  //Serial.println(freeRam());
  unsigned long end_time = millis();
  unsigned long elapse = end_time - start_time;
  if (elapse >= 10000){
    //Every 10 seconds will represent an hour
    start_time = millis();
    hour += elapse/10000;
    if (hour > 24)
      hour = 0;
    if (roomOn){
      //mainRoom.cycle(hour);
      //mainRoom->printAll();
    }
  }
  if (conn){
    // Make a HTTP request:
    //Example of different GETS client.println("GET /changed-brightness?r=1 HTTP/1.1");
    //getBrightChange(client);
    RoomClass * helper = new RoomClass();
    *helper = getRoom(client,roomOn, *helper);
    helper->printAll();
    //mainRoom = *helper;
    //mainRoom.printAll();
  }
}
