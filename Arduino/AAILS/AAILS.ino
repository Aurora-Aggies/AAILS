#include <EEPROM.h>
#include "aails.h"
#include "parser.h"
#include <SPI.h>
#include <SD.h>
#include <Ethernet2.h>


boolean roomOn = false;
boolean sdOn = false;
boolean updateArduino = true;
unsigned long start_time;
byte hour;
RoomClass mainRoom;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); //Initializes Serial Communications
  /*while (!Serial){
    ; //Wait for Serial to connect
  }*/
  //Serial.println(freeRam()); //Prints available ram upon startup

  //These lines just allow SD to be accessed
  pinMode(53,OUTPUT);
  digitalWrite(10,HIGH);

  if (SD.begin(4)) sdOn = true; //SD is on
  if (!SD.exists("time.txt") && sdOn) {
    //Create time.txt if it doesn't already exist
    File f;
    f = SD.open("time.txt",FILE_WRITE);
    f.close();
  }

  //If room is held begin intialization
  if (EEPROM.read(0) == 1){
    roomOn = true;
    EEPROM.get(1,mainRoom); //Gets RoomClass
    if (sdOn){
      File f;
      f = SD.open("time.txt",FILE_READ);
      while (f.available())
        hour = f.read();
      f.close();
      mainRoom.updateTime(hour);
    } else {
      hour = 0;
    }
    if (mainRoom.get_br() == 0.0)
      mainRoom.set_br(1.0);
  }

  //Start Ethernet Connection
  byte mac[] = { 0x2C, 0xF7, 0xF1, 0x08, 0x05, 0x4D };
  byte ip[] = { 192, 168, 1, 3 }; //IP of shield
  Ethernet.begin(mac,ip);
  delay(500);

  EthernetClient client;
  IPAddress server(192, 168, 1, 2); //IP of server
  if (!client.connect(server,3000))
    netErrorAnim(2);

  if (!roomOn) hour = 0; //if room isn't on hour is 0
  start_time = 0;
}

void loop() {
  unsigned long end_time = millis();
  unsigned long elapse = end_time - start_time;
  boolean room_change = false;
  boolean bright_change = false;
  //Server Connection handling
  EthernetClient client;
  IPAddress server(192, 168, 1, 2); //IP of server
  if (client.connect(server,3000)){
    client.stop();
    //Get all changes happening
    room_change = getChanges(server,0);
    bright_change = getChanges(server,1);
    if (updateArduino){
      Serial.println("Updating Arduino...");
      hour = updateTime(server);
      mainRoom.updateTime(hour);
      getRoom(roomOn, mainRoom, server); 
      changeBr(mainRoom,server);
      updateArduino = false;
    }
    if (room_change || !roomOn)
      getRoom(roomOn, mainRoom, server); //change room values
    if (bright_change && roomOn)
      changeBr(mainRoom,server); //change brightness values
  } else {
    //if can't connect to network, update time upon reconnect
    updateArduino = true;
    if (!roomOn) netErrorAnim(4);
  }

  //Time & cycle handler
  if (elapse >= 5000){
    //Every 5 seconds will represent an hour
    start_time = millis();
    hour += elapse/5000; //update hour
    elapse -= 5000; //get any leftover time
    if (hour > 23){
      mainRoom.cycle(hour,elapse); //cycle function
      hour = 0;
    }
    if (sdOn && roomOn){
      //save the hour
      SD.remove("time.txt");
      File f;
      f = SD.open("time.txt",FILE_WRITE);
      f.write(hour);
      f.close();
    }
  }
  if (roomOn)
    mainRoom.cycle(hour,elapse); //cycle function
}
