#include "parser.h"


String parseRequest(String x){
	int start = x.lastIndexOf("<html>") + 8;
	int end = x.lastIndexOf("</html>") - 1;
	x = x.substring(start,end);
	x.trim();
	//Serial.println(x);
	return x;
}

RoomClass getRoom(EthernetClient client, boolean  &bs, RoomClass &rc){
	client.println("GET / HTTP/1.1");
    client.println("Host: www.google.com");
    client.println("Connection: close");
    client.println();
	
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
		Serial.println("Success");
		int sz = root["Cycles"].size(); //Stores how many cycles we need
		Serial.println(c);
		int t[sz]; //Stores color temps for daily cycle
		int b[sz]; //Stores intensity of temp for daily cycle
		int d[sz]; //Stores duration of temp for daily cycle
		int s[sz]; //Stores start times 
		int e[sz]; //Stores end times 
		for (int i = 0; i < sz; i++){ //Sets values
			t[i] = root["Cycles"][i]["T"];
			b[i] = root["Cycles"][i]["L"];
			d[i] = root["Cycles"][i]["P"];
			s[i] = root["Cycles"][i]["S"];
			e[i] = root["Cycles"][i]["E"];
		}
		//int ** save = {t,b,d,s,e}; //idea
		RoomClass room; //Creates room
		room.initCycle(t,b,d,s,e); //Initializes the cycle with these values
		
		bs = true; //change boolean so we know RoomClass is initialized
		EEPROM.update(0,1); //EEPROM store bool above
		EEPROM.put(1,c.length() + 3); //EEPROM  store JSON string size
		//Serial.println(c.length() + 3);
		for (int i = 3; i < c.length() + 3; i++)
			EEPROM.update(i,c.charAt(i-3)); //EEPROM JSON string
		return room;
    } else {
		RoomClass room;
		int a[0];
		int b[0];
		int c[0];
		int d[0];
		int e[0];
		room.initCycle(a,b,c,d,e);
		rc = room;
		return rc;
	}
}

void getBrightChange(EthernetClient client){
	String rs;
	client.println("GET /change-brightness.html HTTP/1.1");
    client.println("Host: www.google.com");
    client.println("Connection: close");
    client.println();
    while (client.available() > 0){
        char g = client.read();
        rs += g;
    }
    if (!client.connected()){
        String command = parseRequest(rs);
        if (command.equals("True"))
			Serial.println("Yay!");
     }
}

RoomClass computeRoom(String r){
	StaticJsonBuffer<1000> jsonBuffer; //Buffer to hold Json objects
    JsonObject& root = jsonBuffer.parseObject(r); //Parses JsonObject
	Serial.println(r);
	if (root.success()){
		int sz = root["Cycles"].size(); //Stores how many cycles we need
		int t[sz]; //Stores color temps for daily cycle
		int b[sz]; //Stores intensity of temp for daily cycle
		int d[sz]; //Stores duration of temp for daily cycle
		int s[sz]; //Stores start times 
		int e[sz]; //Stores end times 
		for (int i = 0; i < sz; i++){ //Sets values
			t[i] = root["Cycles"][i]["T"];
			b[i] = root["Cycles"][i]["L"];
			d[i] = root["Cycles"][i]["P"];
			s[i] = root["Cycles"][i]["S"];
			e[i] = root["Cycles"][i]["E"];
		}
		RoomClass room; //Creates room
		room.initCycle(t,b,d,s,e); //Initializes the cycle with these values
		return room;
	} else {
		Serial.println("JSON parsing failed");
		RoomClass room;
		return room;
	}
}

int freeRam () 
{
  //Prints available SRAM
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}