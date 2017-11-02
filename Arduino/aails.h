#include "Arduino.h"
#include "tempcolor.h"
#include <Adafruit_NeoPixel.h>
#include <avr/pgmspace.h>

#ifndef AAILS_H
#define AAILS_H

class RoomClass{
	Adafruit_NeoPixel light = Adafruit_NeoPixel(60, 6, NEO_GRB + NEO_KHZ800);
	int * t; //Stores temps
	int * br; //Stores brightnesses
	int * day; //Stores percentages of the day
	int * start;
	int * end;
	byte currentPhase; //Stores the current phase
	
	public:
	void initCycle(int tmp [], int bright [], int perc [], int st [], int ed []); //initializes the cycle
	void cycle(int hour); //Goes to next phase in cycle (Call consecutively if possible)
	void printAll();
};

#endif
