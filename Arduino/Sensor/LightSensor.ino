#include <SPI.h>
#include <Ethernet2.h>
#include <Wire.h>
#include <Adafruit_TCS34725.h>



IPAddress server(192,168,1,5);
byte mac[] = { 0x2C, 0xF7, 0xF1, 0x08, 0x0B, 0xD2 };



/* Initialise with default values (int time = 2.4ms, gain = 1x) */
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_2_4MS, TCS34725_GAIN_60X);

uint16_t r, g, b, c, colorTemp, lux = 0;  // define values for color sensor readings, initialize to 0

int sensor = 8;  // digital pin 8 powers sensor


void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // set pin to power sensorB
  pinMode(sensor, OUTPUT);

  //Start Ethernet Connection
  
  Ethernet.begin(mac);
  delay(1000);
  
}

void loop() {
  
  EthernetClient client;
  String rs = "r=1";

  digitalWrite(sensor, HIGH);  // set "pin" to high, powering the sensor
  delay(250);
  Serial.print("SENSOR = ");
  
  // collect data from color sensor
  tcs.getRawData(&r, &g, &b, &c);
  colorTemp = tcs.calculateColorTemperature(r, g, b);
  lux = tcs.calculateLux(r, g, b);

  rs += "&red=";
  rs += (int) (r / 1024.0 * 255);
  rs += "&green=";
  rs += (int) (g / 1024.0 * 255);
  rs += "&blue=";
  rs += (int) (b / 1024.0 * 255);
  rs += "&temp=";
  rs += colorTemp;
  rs += "&lux="; 
  rs += lux;

  //rs = "34567890";

  //rs += colorTemp;

// display data
/*  Serial.print("Color Temp: "); Serial.print(colorTemp); Serial.print(" K - ");
  Serial.print("Lux: "); Serial.print(lux); Serial.print(" - ");
  Serial.print("R: "); Serial.print(r); Serial.print(" ");
  Serial.print("G: "); Serial.print((int) (g / 1024.0 * 255)); Serial.print(" ");
  Serial.print("B: "); Serial.print((int) (b / 1024.0 * 255)); Serial.print(" ");
  Serial.print("C: "); Serial.print(c); Serial.print(" ");
  Serial.println(" ");

  Serial.println(rs);
*/
  
  Serial.println("Connecting...");
  if ( client.connect(server, 3000) ) {
    Serial.println("Connected!");
    
    client.println("POST /sensor-data HTTP/1.1");
    client.println("From: sensor");
    client.println("User-Agent: HTTPTool/1.1");
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.println(rs);
    client.println("Content-Length: " + rs.length());
    client.println("Connection: close");
    client.println();
  }
  delay(5000);
  
  digitalWrite(sensor, LOW);

}


