#include "aails.h"
#include <ArduinoJson.h>
#include <avr/pgmspace.h>

boolean ini = true; //Bool for initialization only

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); //Initializes Serial Communications
  Serial.println(freeRam()); //Prints available ram upon startup
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available() > 0){
      char r [] ="{\"Cycles\":[{\"T\":2700,\"L\":50,\"P\":10},{\"T\":3500,\"L\":80,\"P\":10},{\"T\":5000,\"L\":90,\"P\":10},{\"T\":6000,\"L\":100,\"P\":10},{\"T\":4200,\"L\":75,\"P\":10},{\"T\":3500,\"L\":50,\"P\":10},{\"T\":2200,\"L\":20,\"P\":40}]}";
      StaticJsonBuffer<500> jsonBuffer; //Buffer to hold Json objects
      JsonObject& root = jsonBuffer.parseObject(r); //Parses JsonObject
      RoomClass room = RoomClass(); //Creates and initializes room
      if (root.success() && ini) {
        int t [7]; //Stores color temps for daily cycle
        int b[7]; //Stores intensity of temp for daily cycle
        int d[7]; //Stores duration of temp for daily cycle
        for (byte i = 0; i < 7; i++){
          //Sets values
          t[i] = root["Cycles"][i]["T"];
          b[i] = root["Cycles"][i]["L"];
          d[i] = root["Cycles"][i]["P"];
        }
        room.initCycle(t,b,d); //Initializes the cycle with these values
        ini = false; //Don't run through initalization again
      }else {
        room.cycle(); //Runs next phase of cycle every call
      }
   }
}


int freeRam () 
{
  //Prints available SRAM
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}
