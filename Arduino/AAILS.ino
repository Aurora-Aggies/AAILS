#include "tempcolor.h"
#include "aails.h"
#include <ArduinoJson.h>

System * sys;
DynamicJsonBuffer jsonBuffer(200);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  System temp = System(5);
  sys = &temp;
  sys->getRoom(0)->set(6500,25);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available() > 0){
    String response = Serial.readString();
    JsonObject &root = jsonBuffer.parseObject(response);
    String p = root["cycles"];
    Serial.println(p);
  }

}
