#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <SPI.h>
#include <MFRC522.h>
#include <ESP8266WiFi.h>    
#include <WiFiManager.h> 
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#define Buzzer D3
#define Relay 1
#define ON_Board_LED 2
#define SS_PIN D2  //--> SDA / SS is connected to pinout D2
#define RST_PIN D1  //--> RST is connected to pinout D1
MFRC522 mfrc522(SS_PIN, RST_PIN);  //--> Create MFRC522 instance.

ESP8266WebServer server(80);  //--> Server on port 80

int readsuccess;
byte readcard[4];
char str[32] = "";
String StrUID;
String door="&door_id=1";
String card_id_method="&card_id=";

//-----------------------------------------------------------------------------------------------SETUP--------------------------------------------------------------------------------------//
void setup() {
  Serial.begin(115200); //--> Initialize serial communications with the PC
  SPI.begin();      //--> Init SPI bus
  mfrc522.PCD_Init(); //--> Init MFRC522 card
  delay(500);
  ArduinoOTA.begin();
  
  pinMode(Buzzer, OUTPUT);
  pinMode(ON_Board_LED, OUTPUT);
  pinMode(Relay, OUTPUT);

}


//-----------------------------------------------------------------------------------------------LOOP---------------------------------------------------------------------------------------//
void loop() {
  ArduinoOTA.handle();
  digitalWrite(ON_Board_LED, HIGH); //--> Turn off Led On Board
  
  while(WiFi.status() != WL_CONNECTED){
    WiFiManager wifiManager;
    digitalWrite(ON_Board_LED, LOW);  
    wifiManager.autoConnect("AutoConnectAP");
  }
  digitalWrite(ON_Board_LED, HIGH); //turn off the led 
  
  
  readsuccess = getid();

  if (readsuccess) {
    digitalWrite(ON_Board_LED, LOW);
    HTTPClient http;    //Declare object of class HTTPClient

    String UIDresultSend, url;
    UIDresultSend = StrUID;

    url = "http://192.168.1.31/rfapi.php?method=rfentry";
    url += card_id_method+UIDresultSend+door;

    http.begin(url);  //Specify request destination
    http.addHeader("Content-Type", "application/x-www-form-urlencoded"); //Specify content-type header

    int httpCode = http.POST(url);   //Send the request
    String payload = http.getString();    //Get the response payload

    if(payload=="1"){
        digitalWrite(Relay, HIGH);
        digitalWrite(Buzzer, HIGH);
        delay(200);
        digitalWrite(Relay, LOW);
        digitalWrite(Buzzer, LOW);
      }
    http.end();  //Close connection
    delay(1000);
    digitalWrite(ON_Board_LED, HIGH);

  }
}

//----------------------------------------Procedure for reading and obtaining a UID from a card or keychain---------------------------------------------------------------------------------//
int getid() {
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return 0;
  }
  if (!mfrc522.PICC_ReadCardSerial()) {
    return 0;
  }


  Serial.print("THE UID OF THE SCANNED CARD IS : ");

  for (int i = 0; i < 4; i++) {
    readcard[i] = mfrc522.uid.uidByte[i]; //storing the UID of the tag in readcard
    array_to_string(readcard, 4, str);
    StrUID = str;
  }
  mfrc522.PICC_HaltA();
  return 1;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

//----------------------------------------Procedure to change the result of reading an array UID into a string------------------------------------------------------------------------------//
void array_to_string(byte array[], unsigned int len, char buffer[]) {
  for (unsigned int i = 0; i < len; i++)
  {
    byte nib1 = (array[i] >> 4) & 0x0F;
    byte nib2 = (array[i] >> 0) & 0x0F;
    buffer[i * 2 + 0] = nib1  < 0xA ? '0' + nib1  : 'A' + nib1  - 0xA;
    buffer[i * 2 + 1] = nib2  < 0xA ? '0' + nib2  : 'A' + nib2  - 0xA;
  }
  buffer[len * 2] = '\0';
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
