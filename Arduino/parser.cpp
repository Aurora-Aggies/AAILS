#include "parser.h"


String parseRequest(String x){
	int start = x.lastIndexOf("<html>") + 8;
	int end = x.lastIndexOf("</html>") - 1;
	x = x.substring(start,end);
	x.trim();
	return x;
}

void getRoom(EthernetClient client, boolean  &bs, RoomClass &rc){
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
    StaticJsonBuffer<1000> jsonBuffer; //Buffer to hold Json objects
    JsonObject& root = jsonBuffer.parseObject(c); //Parses JsonObject
    if (root.success()) {
		byte sz = root["Cycles"].size(); //Stores how many cycles we need
		int t[sz]; //Stores color temps for daily cycle
		int b[sz]; //Stores intensity of temp for daily cycle
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
		EEPROM.put(2,sizeof(room)); //EEPROM  store JSON string size
		EEPROM.put(4,room); //EEPROM store room class
    }
}

boolean getBrightChange(EthernetClient client, IPAddress server){
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
        if (command.equals("True")){
			Serial.println("Yeah");
			return true;
		}
		else {
			return false;
		}
		client.connect(server, 8080);
    }
}

int freeRam () 
{
  //Prints available SRAM
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}