#include "aails.h"
//Let's say a day is 30 seconds (30,000 ms)
const int PROGMEM DAY_LENGTH = 30000;

void RoomClass::initCycle(int tmp [], int bright [], byte st [], byte ed [], byte sz){
	Adafruit_NeoPixel light = Adafruit_NeoPixel(60, 6, NEO_GRB + NEO_KHZ800);
	light.begin(); //Allows lights to be modified
	temp.RGB(); //Initializes coloTemp class to RGB values instead of GRB
	size = sz;
	for (byte i=0; i<sz;i++){
		t[i] = tmp[i]; //sets the color temps for this instance
		br[i] = bright[i]; //sets the brightnesses for this instance
		start[i] = st[i]; //sets start times
		end[i] = ed[i]; //sets end times
	}
	//Set every pixel to first color temp and brightness value
	for(byte i=0;i<60;i++)
		light.setPixelColor(i, temp.color(t[0],br[0]));
	light.show(); //Lights will light up with this
	currentPhase = 0; //Initialize current phase
	//delay((DAY_LENGTH * (day[currentPhase] * .01))); //Delay the appropriate amount for cycle
}

void RoomClass::cycle(byte hour){
    //Serial.println(hour);
	byte ec = end[currentPhase];
	if (hour == ec){
		//Serial.println("Switch");
		currentPhase++;
		if (currentPhase >= size) currentPhase = 0;
		Adafruit_NeoPixel light = Adafruit_NeoPixel(60, 6, NEO_GRB + NEO_KHZ800);
		light.begin();
		for(byte i=0;i<60;i++)
			light.setPixelColor(i, temp.color(t[currentPhase],br[currentPhase]));
		light.show();
	}
	
}

/*void RoomClass::printAll(){
	for (int i = 0; i < size; i++){
		Serial.println(t[i]);
		Serial.println(br[i]);
		Serial.println(start[i]);
		Serial.println(end[i]);
	}
}*/
