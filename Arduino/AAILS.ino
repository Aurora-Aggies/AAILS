#include <EEPROM.h>

#include "aails.h"
#include "parser.h"
#include <SPI.h>
#include <SD.h>
#include <Ethernet2.h>
//#include <ArduinoJson.h>
#include <avr/pgmspace.h>

boolean conn = false;
boolean roomOn = false;
boolean sdOn = false;
unsigned long start_time;
byte hour;
byte mac[] = { 0x2C, 0xF7, 0xF1, 0x08, 0x05, 0x4D };
byte brightness;
RoomClass mainRoom;
IPAddress server(192, 168, 137, 1);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); //Initializes Serial Communications
  /*while (!Serial){
    ; //Wait for Serial to connect
  }*/
  //Serial.println(freeRam()); //Prints available ram upon startup

  pinMode(53,OUTPUT);
  digitalWrite(10,HIGH);

  if (SD.begin(4)) sdOn = true;
  if (!SD.exists("time.txt") && sdOn) {
    File f;
    f = SD.open("time.txt",FILE_WRITE);
    f.close();
  }
  
  if (EEPROM.read(0) == 1){
    roomOn = true;
    if (sdOn){
      File f;
      f = SD.open("time.txt",FILE_READ);
      while (f.available())
        hour = f.read();
      f.close();
    } else {
      hour = 0;
    }
    EEPROM.get(2,mainRoom);
    brightness = EEPROM.read(1);
  }

  //Start Ethernet Connection
  Ethernet.begin(mac);
  delay(1000);

  EthernetClient client;
  if (client.connect(server, 8080)) {
    conn = true;
    client.stop();
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
      if (sdOn){
        SD.remove("time.txt");
        File f;
        f = SD.open("time.txt",FILE_WRITE);
        f.write(hour);
        f.close();
      }
      mainRoom.cycle(hour);
    }
  }
  if (conn){
    // Make a HTTP request:
    //Example of different GETS client.println("GET /changed-brightness?r=1 HTTP/1.1");
    boolean room_change = getRoomChange(server);
    boolean bright_change = getBrightChange(server);
    if (room_change){
      room_change = false;
      getRoom(roomOn, mainRoom, server);
    }
    if (bright_change && roomOn){
      bright_change = false;
      Serial.println("changing brightness");
      changeBr(mainRoom,server);
    }
  }
}
