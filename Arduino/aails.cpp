#include "aails.h"
//Let's say a day is 30 seconds (30,000 ms)
const int PROGMEM DAY_LENGTH = 30000;

void RoomClass::initCycle(int tmp [], int bright [], int perc [], int st [], int ed []){
	//Initializes the cycle. Should be called ASAP after constructor
	light.begin(); //Allows lights to be modified
	temp.RGB(); //Initializes coloTemp class to RGB values instead of GRB
	t = tmp; //sets the color temps for this instance
	br = bright; //sets the brightnesses for this instance
	day = perc; //sets the percentages of this instance
	start = st; //sets start times
	end = ed; //sets end times 
	for(byte i=0;i<60;i++){
		//Set every pixel to first color temp and brightness value
		light.setPixelColor(i, temp.color(t[0],br[0]));
	}
	light.show(); //Lights will light up with this
	currentPhase = 0; //Initialize current phase
	//delay((DAY_LENGTH * (day[currentPhase] * .01))); //Delay the appropriate amount for cycle
}

void RoomClass::cycle(int hour){
	//Serial.println("Hour");
    Serial.println(hour);
	int ec = end[0];
	//Serial.println(t[0]);
	if (hour == ec){
		Serial.println("Switch");
		currentPhase++;
		Serial.println(currentPhase);
		for(byte i=0;i<60;i++)
			light.setPixelColor(i, temp.color(t[currentPhase],br[currentPhase]));
		light.show(); //Lights will light up with this
	}
	
}

void RoomClass::printAll(){
	for (byte i = 0; i < 7; i++){
		Serial.println(t[i]);
		Serial.println(br[i]);
		Serial.println(day[i]);
	}
}
