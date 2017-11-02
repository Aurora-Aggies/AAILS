#include "Arduino.h"
#include <ArduinoJson.h>
#include "aails.h"
#include <SPI.h>
#include <Ethernet2.h>
#include <EEPROM.h>

#ifndef PARSER_H
#define PARSER_H
String parseRequest(String x);
void getRoom(EthernetClient client, boolean &b, RoomClass &rc);
void getBrightChange(EthernetClient client);
void computeRoom(String r,RoomClass &rc);
int freeRam();
#endif