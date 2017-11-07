#include "aails.h"

void RoomClass::initCycle(int tmp [], byte bright [], byte st [], byte ed [], byte sz){
	Adafruit_NeoPixel light = Adafruit_NeoPixel(60, 6, NEO_GRB + NEO_KHZ800);
	light.begin(); //Allows lights to be modified
	temp.RGB(); //Initializes coloTemp class to RGB values instead of GRB
	size = sz;
	br_hold = 0; //default brightness
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
}

void RoomClass::cycle(byte hour, unsigned long elapse){
	byte ec = end[currentPhase];
	float eratio = (float)elapse/5000;
	Serial.print("Hour: ");
	Serial.println(hour);
	while(hour > ec){
		currentPhase++;
		ec = end[currentPhase];
	}
	if (hour == ec){
		currentPhase++;
		if (currentPhase >= size) currentPhase = 0;
		Serial.println(t[currentPhase]);
		Adafruit_NeoPixel light = Adafruit_NeoPixel(60, 6, NEO_GRB + NEO_KHZ800);
		light.begin();
		for(byte i=0;i<60;i++)
			light.setPixelColor(i, temp.color(t[currentPhase],br[currentPhase]));
		light.setBrightness(br_hold); //Keep eye
		light.show();
	} else {
		//Used to transition from one phase to the next
		int dur = end[currentPhase] - start[currentPhase];
		int tdiff = t[currentPhase + 1] - t[currentPhase];
		int bdiff = br[currentPhase + 1] - br[currentPhase];
		if (currentPhase + 1 >= size){
			tdiff = t[0] - t[currentPhase];
			bdiff = br[0] - br[currentPhase];
		}
		float mult = ((float)hour + eratio) - start[currentPhase];
		tincr = (tdiff/dur) * mult;
		bincr = (bdiff/dur) * mult;
		
		Adafruit_NeoPixel light = Adafruit_NeoPixel(60, 6, NEO_GRB + NEO_KHZ800);
		light.begin();
		for(byte i=0;i<60;i++)
			light.setPixelColor(i, temp.color(t[currentPhase] + tincr,br[currentPhase] + bincr));
		light.setBrightness(br_hold); //Keep eye
		light.show();
	}
	
}

void RoomClass::set_br(byte b){
	br_hold = b;
	Adafruit_NeoPixel light = Adafruit_NeoPixel(60, 6, NEO_GRB + NEO_KHZ800);
	light.begin();
	for(byte i=0;i<60;i++)
		light.setPixelColor(i, temp.color(t[currentPhase],br[currentPhase]));
	light.setBrightness(b);
	light.show();
}

void RoomClass::updateTime(byte hour){
	currentPhase = 0;
	byte ec = end[currentPhase];
	while(hour >= ec){
		currentPhase++;
		ec = end[currentPhase];
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
