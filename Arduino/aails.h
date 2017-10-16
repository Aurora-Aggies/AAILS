#include "Arduino.h"
#include "tempcolor.h"
#include <Adafruit_NeoPixel.h>
#include <avr/pgmspace.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#ifndef AAILS_H
#define AAILS_H

#define PIN            6

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      60

class RoomClass{
	PROGMEM Adafruit_NeoPixel light = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
	int * t; //Stores temps
	int * br; //Stores brightnesses
	int * day; //Stores percentages of the day
	int currentPhase; //Stores the current phase
	
	public:
	RoomClass();
	void showPhase(int num); //Shows current phase
	void set(int tc, int br); //Sets to certain temp and brightness
	void off(); //turns lights off
	void printAll(); //prints all cycle values
	void initCycle(int tmp [], int bright [], int perc []); //initializes the cycle
	void cycle(); //Goes to next phase in cycle (Call consecutively if possible)
};

#endif
