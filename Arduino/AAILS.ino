#include "aails.h"
#include <ArduinoJson.h>

System * sys;

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
    //Serial.println("Hello");
    int n = Serial.parseInt();
    Serial.println(n);
    if (n == 0) sys->getRoom(n)->set(6500,25);
    if (n == 1) sys->getRoom(n)->set(5000,50);
    if (n == 2) sys->getRoom(n)->set(7000,25);
    if (n == 3) sys->getRoom(n)->set(1500,50);
    if (n == 4) sys->getRoom(n)->set(2500,50);
    if (n == 5){
      const char r [] PROGMEM = "{\"Cycles\":[{\"ID\":\"Sunset\",\"Temperature\":2700,\"Lumens\":1500,\"Percentage\":0.1},{\"ID\":\"Dawn\",\"Temperature\":3500,\"Lumens\":1700,\"Percentage\":0.1},{\"ID\":\"Overcast\",\"Temperature\":5000,\"Lumens\":2000,\"Percentage\":0.1},{\"ID\":\"Daylight\",\"Temperature\":6000,\"Lumens\":2200,\"Percentage\":0.1},{\"ID\":\"MidAfternoon\",\"Temperature\":4200,\"Lumens\":1900,\"Percentage\":0.1},{\"ID\":\"Dusk\",\"Temperature\":3500,\"Lumens\":1700,\"Percentage\":0.1},{\"ID\":\"Night\",\"Temperature\":2200,\"Lumens\":20,\"Percentage\":0.4}],\"HoursPerDay\":24}";
      StaticJsonBuffer<600> jsonBuffer;
      JsonObject& root = jsonBuffer.parseObject(r);
      if (!root.success()) {
        Serial.println("parseObject() failed");
      } else {
      const char* p = root["Cycles"][0]["ID"];
      Serial.println(p);
      }
    }
    sys->switchTo(n);
  }

}
