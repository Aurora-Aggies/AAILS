#include "Arduino.h"
#include <ArduinoJson.h>
#include "aails.h"
#include <SPI.h>
#include <Ethernet2.h>
#include <EEPROM.h>

#ifndef PARSER_H
#define PARSER_H
String parseRequest(String x);
void getRoom(boolean &b, RoomClass &rc, IPAddress server);
void changeBr(RoomClass &rc, IPAddress server);
boolean getChanges(IPAddress server, byte b);
byte updateTime(IPAddress server);
int freeRam();
#endif
