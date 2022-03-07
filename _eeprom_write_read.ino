#include <EEPROM.h>
 
void clearEEPROM()
{
  for(int i=0; i<4096; i++)
  {
    EEPROM.write(i,0);
    delay(2);
  }
  EEPROM.commit();
  Serial.println("EEPROM succesfully cleared.");
}

void writestrEEPROM(int address, String str)
{
  for(int i=0;i<str.length();i++)
  {
    EEPROM.write(address+i, str[i]); //Write one by one with starting address of 0x0F
    delay(2);
  }
  EEPROM.commit();    //Store data to EEPROM
  Serial.print("Data: ");
  Serial.print(str);
  Serial.println(" successfully writen to EEPROM.");
}

void readstrEEPROM(int address)
{
  String str;
  for(int i=0;i<4096;i++) 
  {
    if(char(EEPROM.read(address+i)=='*')) break;
    str = str + char(EEPROM.read(address+i)); //Read one by one with starting address of
    delay(2);
  }
  Serial.println(str);  
}
void setup()
{
  EEPROM.begin(4096);  //Initialize EEPROM
  Serial.begin(115200);
  delay(1000); 
  writestrEEPROM(4060,"1A5D6S9A*");
//  writestrEEPROM(30,"F5E9F5D4");

//  readstrEEPROM(30);

//  clearEEPROM();
    readstrEEPROM(4060);
}
void loop()
{ 
  
}
