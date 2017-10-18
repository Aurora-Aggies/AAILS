#include "aails.h"
//Let's say a day is 30 seconds (30,000 ms)
const int PROGMEM DAY_LENGTH = 30000;
	
RoomClass::RoomClass(){
	//Constructor
	light.begin(); //Allows lights to be modified
	temp.RGB(); //Initializes coloTemp class to RGB values instead of GRB
}

void RoomClass::showPhase(int num){
	//Shows desired phase within the cycle
	if (num < 7 || num > -1){
		currentPhase = num;
		for(byte i=0;i<NUMPIXELS;i++)
			light.setPixelColor(i, temp.color(t[currentPhase],br[currentPhase]));
		light.show();
		delay((DAY_LENGTH * (day[currentPhase] * .01)));
	}
}

void RoomClass::set(int tc, int brt){
	for(byte i=0;i<NUMPIXELS;i++){
		//Typical way to set lights (every px) to a certain color and brightness
		light.setPixelColor(i, temp.color(tc,brt));
	}
	light.show();
}

void RoomClass::off(){
	//Turns lights off (color values set to 0)
	for(byte i=0;i<NUMPIXELS;i++)
		light.setPixelColor(i, 0,0,0);	
	light.show();
}

void RoomClass::printAll(){
	//Prints all values in the cycle
	for (byte i = 0; i < 7; i++){
		Serial.println(t[i]);
		Serial.println(br[i]);
		Serial.println(day[i]);
		Serial.println(i);
	}
}

void RoomClass::initCycle(int tmp [], int bright [], int perc []){
	//Initializes the cycle. Should be called ASAP after constructor
	t = tmp; //sets the color temps for this instance
	br = bright; //sets the brightnesses for this instance
	day = perc; //sets the percentages of this instance
	for(byte i=0;i<NUMPIXELS;i++){
		//Set every pixel to first color temp and brightness value
		light.setPixelColor(i, temp.color(t[0],br[0]));
	}
	light.show(); //Lights will light up with this
	currentPhase = 0; //Initialize current phase
	delay((DAY_LENGTH * (day[currentPhase] * .01))); //Delay the appropriate amount for cycle
}

void RoomClass::cycle(){
	if (currentPhase + 1 < 7){
		currentPhase++; //increment to next phase
		for(byte i=0;i<NUMPIXELS;i++){
			//set to next phase
			light.setPixelColor(i, temp.color(t[currentPhase],br[currentPhase]));
		}
		light.show(); //show next phase
		delay((DAY_LENGTH * (day[currentPhase] * .01))); //delay appropriately
	}
	else {
		//End of phase case. Initialize back to start.
		Serial.println("End of Phase");
		currentPhase = 0; //Initializing back to start
		for(byte i=0;i<NUMPIXELS;i++){
			light.setPixelColor(i, temp.color(t[0],br[0]));
		}
		light.show();
		delay((DAY_LENGTH * (day[currentPhase] * .01)));
	}
}
