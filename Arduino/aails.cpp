#include "aails.h"

RoomClass::RoomClass(){
	light.begin();
}

void RoomClass::show(){
	light.show();
}

void RoomClass::set(int tc, int br){
	temp.RGB();
	for(int i=0;i<NUMPIXELS;i++){
		//Serial.println(i);
		light.setPixelColor(i, temp.color(tc,br));
	}
}

void RoomClass::off(){
	for(int i=0;i<NUMPIXELS;i++)
		light.setPixelColor(i, 0,0,0);	
	light.show();
}

void System::switchTo(int r){
	rooms[r].show();
}


System::System(int n){
	rooms = new RoomClass[n]();
	numOfRooms = n;
	for (int i = 0; i < n; i++){
		rooms[i].set(2500,25);
	}
	rooms[0].show();
}

RoomClass * System::getRoom(int r){
	return &rooms[r];
}
