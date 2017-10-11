#include "tempcolor.h"
#include "aails.h"
#include <ArduinoJson.h>

System * sys;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  System temp = System(5);
  sys = &temp;
  sys->getRoom(0)->set(6500,25);
  /*char r [] = "Hello World";
  StaticJsonBuffer<600> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(r);
  if (!root.success()) {
    Serial.println("parseObject() failed");
  } else {
  const char* p = root["Cycles"][0]["ID"];
  Serial.println(p);
  }*/
  
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available() > 0){
    //Serial.println("Hello");
    int n = Serial.parseInt();
    Serial.println(n);
    if (n == 0) sys->getRoom(n)->set(6500,25);
    if (n == 1) sys->getRoom(n)->set(5000,50);
    if (n == 2) sys->getRoom(n)->set(7000,25);
    if (n == 3) sys->getRoom(n)->set(1500,50);
    if (n == 4) sys->getRoom(n)->set(2500,50);
    sys->switchTo(n);
  }

}
