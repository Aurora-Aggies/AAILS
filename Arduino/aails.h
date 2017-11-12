#include "Arduino.h"
#include "tempcolor.h"
#include <Adafruit_NeoPixel.h>
#include <avr/pgmspace.h>

#ifndef AAILS_H
#define AAILS_H

class RoomClass{
	int t [24]; //Stores temps
	byte br [24]; //Stores brightnesses
	byte start [24];
	byte end [24];
	byte size;
	byte currentPhase; //Stores the current phase
	float br_scale;
	int tincr; //temp incrementor
	int bincr; //brightness incrementor
	
	public:
	void initCycle(int tmp [], byte bright [], byte st [], byte ed [], byte sz); //initializes the cycle
	void cycle(byte hour, unsigned long elapse); //Goes to next phase in cycle (Call consecutively if possible)
	void set_br(float b);
	float get_br();
	void updateTime(byte hour);
	//void printAll();
};

void netErrorAnim(byte repeat);

#endif
