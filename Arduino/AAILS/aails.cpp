#include "aails.h"

void RoomClass::initCycle(int tmp [], byte bright [], byte st [], byte ed [], byte sz){
	temp.RGB(); //Initializes coloTemp class to RGB values instead of GRB
	size = sz;
	br_scale = 1.0; //default brightness
	for (byte i=0; i<sz;i++){
		t[i] = tmp[i]; //sets the color temps for this instance
		br[i] = bright[i]; //sets the brightnesses for this instance
		start[i] = st[i]; //sets start times
		end[i] = ed[i]; //sets end times
	}
	currentPhase = 0; //Initialize current phase
}

void RoomClass::cycle(byte hour, unsigned long elapse){
	if (hour > 24) {
		hour = 0;
		currentPhase = 0;
	}
	byte ec = end[currentPhase];
	float eratio = (float)elapse/5000;
	Serial.print("Hour: ");
	Serial.println(hour);
	Serial.print("currentPhase: ");
	Serial.println(currentPhase);
	Serial.print("br_scale: ");
	Serial.println(br_scale);
	while(hour > ec){
		currentPhase++;
		ec = end[currentPhase];
	}
	if (hour == ec){
		currentPhase++;
		if (currentPhase >= size) currentPhase = 0;
		Serial.println(t[currentPhase]);
		Adafruit_NeoPixel light = Adafruit_NeoPixel(60, 6, NEO_GRB + NEO_KHZ800);
		int scale =  br[currentPhase] * br_scale;
		light.begin();
		for(byte i=0;i<60;i++)
			light.setPixelColor(i, temp.color(t[currentPhase],scale));
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
		int scale = (br[currentPhase] + bincr) * br_scale;
		Serial.print("scale: ");
		Serial.println(scale);
		light.begin();
		for(byte i=0;i<60;i++)
			light.setPixelColor(i, temp.color(t[currentPhase] + tincr,scale));
		light.show();
	}
	
}

void RoomClass::set_br(float b){
	br_scale = b;
	Adafruit_NeoPixel light = Adafruit_NeoPixel(60, 6, NEO_GRB + NEO_KHZ800);
	int scale = br[currentPhase] * br_scale;
	Serial.print("scale: ");
	Serial.println(scale);
	light.begin();
	for(byte i=0;i<60;i++)
		light.setPixelColor(i, temp.color(t[currentPhase],scale));
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

float RoomClass::get_br(){
	return br_scale;
}

void netErrorAnim(byte repeat){
	Serial.println("Network not connected");
	Adafruit_NeoPixel light = Adafruit_NeoPixel(60, 6, NEO_GRB + NEO_KHZ800);
	light.begin();
	for (byte k=0; k<repeat; k++){
		for (byte i=0; i < 60; i++){
			for(byte j=0;j<60;j++)
				light.setPixelColor(j,i,0,0);
			light.show();
			delay(10);
		}
		for (byte i=80; i > 0; i--){
			for(byte j=0;j<60;j++)
				light.setPixelColor(j,i,0,0);
			light.show();
			delay(10);
		}
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
