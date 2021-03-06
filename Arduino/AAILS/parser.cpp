#include "parser.h"

const int port = 3000;

String parseRequest(String x){
	int start = x.lastIndexOf(":body:") + 7;
	int end = x.lastIndexOf(":/body:") - 1;
	x = x.substring(start,end);
	x.trim();
	return x;
}

void getRoom(boolean  &bs, RoomClass &rc, IPAddress server){
	EthernetClient client;
	client.connect(server, port);
	client.println("GET /current-room?r=1 HTTP/1.1");
	client.println("Connection: close");
	client.println();
	delay(250);
	
	String rs = "";
    while (client.available() > 0){
        char g = client.read();
        rs += g;
    }
    String c = parseRequest(rs);
	Serial.println(c);
    StaticJsonBuffer<1000> jsonBuffer; //Buffer to hold Json objects
    JsonObject& root = jsonBuffer.parseObject(c); //Parses JsonObject
    if (root.success()) {
		byte sz = root["Cycles"].size(); //Stores how many cycles we need
		int t[sz]; //Stores color temps for daily cycle
		byte b[sz]; //Stores intensity of temp for daily cycle
		byte s[sz]; //Stores start times 
		byte e[sz]; //Stores end times 
		for (byte i = 0; i < sz; i++){ //Sets values
			t[i] = root["Cycles"][i]["T"];
			b[i] = root["Cycles"][i]["L"];
			s[i] = root["Cycles"][i]["S"];
			e[i] = root["Cycles"][i]["E"];
		}
		RoomClass room; //Creates room
		room.initCycle(t,b,s,e,sz); //Initializes the cycle with these values
		rc = room;

		bs = true; //change boolean so we know RoomClass is initialized
		EEPROM.update(0,1); //EEPROM store bool above
		EEPROM.put(1,room); //EEPROM store room class
    }
}

boolean getChanges(IPAddress server, byte b){
	String rs;
	EthernetClient client;
	client.connect(server, port);
	if (b == 0) 
		client.println("GET /changed-room?r=1 HTTP/1.1");
	if (b == 1)
		client.println("GET /changed-brightness?r=1 HTTP/1.1");
	client.println("Connection: close");
	client.println();
	delay(250);
	
    while (client.available() > 0){
        char g = client.read();
        rs += g;
    }
    if (!client.connected()){
        String command = parseRequest(rs);
		client.stop();
        if (command.equals("true"))
			return true;
		else
			return false;
    }
}

void changeBr(RoomClass &rc, IPAddress server){
	EthernetClient client;
	client.connect(server, port);
	client.println("GET /current-brightness?r=1 HTTP/1.1");
	client.println("Connection: close");
	client.println();
	delay(250);
	
	String rs = "";
    while (client.available() > 0){
        char g = client.read();
        rs += g;
    }
	String c = parseRequest(rs);
	float b = c.toFloat();
	rc.set_br(b);
	
}

byte updateTime(IPAddress server){
	EthernetClient client;
	client.connect(server, port);
	client.println("GET /current-time HTTP/1.1");
	client.println("Connection: close");
	client.println();
	delay(250);
	
	String rs = "";
    while (client.available() > 0){
        char g = client.read();
        rs += g;
    }
	String c = parseRequest(rs);
	Serial.print("Hour: ");
	Serial.println(c.toInt());
	byte b = c.toInt();
	return b;
}

int freeRam () 
{
  //Prints available SRAM
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}
