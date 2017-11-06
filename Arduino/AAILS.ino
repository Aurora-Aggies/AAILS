#include <EEPROM.h>
#include "aails.h"
#include "parser.h"
#include <SPI.h>
#include <SD.h>
#include <Ethernet2.h>


boolean roomOn = false;
boolean sdOn = false;
boolean timeupdate = true;
unsigned long start_time;
byte hour;
byte brightness;
RoomClass mainRoom;

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
  byte mac[] = { 0x2C, 0xF7, 0xF1, 0x08, 0x05, 0x4D };
  Ethernet.begin(mac);
  delay(1000);

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
    if (hour > 23)
      hour = 0;
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
  
  EthernetClient client;
  IPAddress server(192, 168, 137, 1);
  if (client.connect(server,8080)){
    Serial.println("Connected");
    client.stop();
    boolean room_change = getChanges(server,0);
    boolean bright_change = getChanges(server,1);
    if (timeupdate){
      Serial.println("Updating time...");
      hour = updateTime(server);
      timeupdate = false;
    }
    if (room_change)
      getRoom(roomOn, mainRoom, server);
    if (bright_change && roomOn)
      changeBr(mainRoom,server);
  } else {
    timeupdate = true;
  }
}
