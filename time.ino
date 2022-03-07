#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

// Replace with your network credentials
const char *ssid     = "orhun";
const char *password = "orhun321";

  WiFiUDP ntpUDP;
  NTPClient timeClient(ntpUDP, "pool.ntp.org");
  
void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  delay(500);
  // Connect to Wi-Fi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  timeClient.begin();
  timeClient.setTimeOffset(10800);
}

void loop() {
  
  gettime();
  
}

void gettime(){
  

  
  timeClient.update();
  unsigned long epochTime = timeClient.getEpochTime();

  int currentHour = timeClient.getHours();
  int currentMinute = timeClient.getMinutes();

  String hour= String(currentHour);
  String minute= String(currentMinute);

  if(hour.length()<2){
    hour ="0"+hour;
    }
  if(minute.length()<2){
    minute ="0"+minute;
    }
    
  //Get a time structure
  struct tm *ptm = gmtime ((time_t *)&epochTime); 
  int monthDay = ptm->tm_mday;
  int currentMonth = ptm->tm_mon+1;
  int currentYear = ptm->tm_year+1900;

  String day= String(monthDay);
  String month= String(currentMonth);
  String year= String(currentYear);
  
  if(day.length()<2){
    day ="0"+day;
    }
  if(month.length()<2){
    month ="0"+month;
    }
 
  //Print complete date:
  String currentDate = day + "."+ month + "." + year + "/" + hour + "." + minute ;
  Serial.print("Current date: ");
  Serial.println(currentDate);
  Serial.println("");
  delay(1000); 
  
  }
