#include <SoftwareSerial.h> 
#include <SparkFunESP8266WiFi.h>
#include <SPI.h>
#include <MFRC522.h>

constexpr uint8_t RST_PIN = 5;     // Configurable, see typical pin layout above
constexpr uint8_t SS_PIN = 10;     // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance

//////////////////////////////
// WiFi Network Definitions //
//////////////////////////////
// Replace these two character strings with the name and
// password of your WiFi network.
const char ssid[] = "NETGEAR01";
const char password[] = "perfectcomet2";
IPAddress server(192,168,1,2);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  SPI.begin();
  while(!Serial);
  if (esp8266.begin()) Serial.println("Wifi Shield activated");
  else Serial.println("Wifi Shield not detected");
  while (esp8266.connect(ssid,password) <= 0) 
  mfrc522.PCD_Init();                                              // Init MFRC522 card
  //mfrc522.PCD_SetRegisterBitMask(mfrc522.RFCfgReg, (0x07<<4));
  Serial.println(F("Read personal data on a MIFARE PICC:")); 
  

}

void loop() {
  String nameTest = "r=1&n=";
  
  //RFID
  MFRC522::MIFARE_Key key;
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;

  byte block;
  byte len;
  MFRC522::StatusCode status;

  if ( ! mfrc522.PICC_IsNewCardPresent())
    return;

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial())
    return;

  Serial.println(F("**Card Detected:**"));
  mfrc522.PICC_DumpDetailsToSerial(&(mfrc522.uid)); //dump some details about the card

  Serial.print(F("Name: "));

  byte buffer1[18];

  block = 4;
  len = 18;

  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 4, &key, &(mfrc522.uid)); //line 834 of MFRC522.cpp file
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Authentication failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }

  status = mfrc522.MIFARE_Read(block, buffer1, &len);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Reading failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }

  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();

  //PRINT FIRST NAME
  for (uint8_t i = 0; i < 16; i++)
  {
    if (buffer1[i] != 32){
      Serial.write(buffer1[i]);
      char c = buffer1[i];
      nameTest += c;
    }
  }
  Serial.print(" ");
  Serial.print(F("Request is: "));
  Serial.println(nameTest);

  //WiFi connection
  ESP8266Client client;
  int tst = client.connect(server,3000);
  if (tst > 0){
    Serial.println(F("Sending to server.."));
    client.println("POST /astronaut-detected HTTP/1.1");
    client.println("From: RFID");
    client.println("User-Agent: HTTPTool/1.1");
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.print("Content-Length: ");
    client.println(nameTest.length());
    client.println();
    client.println(nameTest);
    client.println("Connection: close");
    client.println();
  }
  client.stop();
  Serial.println(F("Done!"));
  delay(500);

}
