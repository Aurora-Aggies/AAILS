#include "Arduino.h"
#include "tempcolor.h"
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#ifndef AAILS_H
#define AAILS_H

#define PIN            6

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      60

class RoomClass{
	int id;
	int px;
	Adafruit_NeoPixel light = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
	
	public:
	RoomClass();
	void show();
	void set(int tc, int br);
	void off();
};

class System {
	RoomClass * rooms;
	int numOfRooms = 0;
	
	public:
	System(int);
	void switchTo(int r);
	RoomClass * getRoom (int r);
};

#endif