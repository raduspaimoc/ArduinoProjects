/*
 * MASTER CONFIG
 */

#include "Wire.h"
#include "MyLibrary.h"
#define SLAVE_ADDR 0x30

SoftwareSerial sf(8, 9); //rx rt
MyLib serialLib =  MyLib(sf);



char string[564];
char inputChar;
uint16_t cal16;
uint8_t cal8;
String aux;

struct data 
{
  float x, y, z;
};


data data;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); 
  Wire.begin(); 
}

void loop() {
  // put your main code here, to run repeatedly:
  while(Serial.available()){   //Wait for something
    inputChar = Serial.read();

    if(inputChar == 't' || inputChar == 'd' || inputChar == 'h'|| inputChar == 'a'){
      
      Serial.print("Request: ");
      Serial.print(inputChar);
      Serial.print(" - Receiving: ");
      serialLib.clearScreen();            

      Wire.beginTransmission(SLAVE_ADDR);
      Wire.write(inputChar);
      Wire.endTransmission();

      switch(inputChar){
        case 't':
                  float t;
                  Wire.requestFrom(SLAVE_ADDR, sizeof(float));
                  Wire.readBytes((byte* )&t, sizeof(float));
                  Serial.println(t);                  
                  aux = "Request: " + String(inputChar) + " - Receiving: " + String(t);
                  String(aux).toCharArray(string, 256);
                  serialLib.print(50, 10, string);                 
                  break;
        case 'd':
                  int distance;
                  Wire.requestFrom(SLAVE_ADDR, sizeof(int));
                  Wire.readBytes((byte* )&distance, sizeof(int));
                  aux = "Request: " + String(inputChar) + " - Receiving: " + String(distance);
                  String(aux).toCharArray(string, 256);
                  serialLib.print(50, 10, string);                  
                  break;
        case 'h':
                  float humidity;
                  Wire.requestFrom(SLAVE_ADDR, sizeof(float));
                  Wire.readBytes((byte* )&humidity, sizeof(float));
                  Serial.println(humidity);
                  aux = "Request: " + String(inputChar) + " - Receiving: " + String(humidity);
                  String(aux).toCharArray(string, 256);
                  serialLib.print(50, 10, string);                  
                  break;
        case 'a': 
                  Wire.requestFrom(SLAVE_ADDR, sizeof(data));
                  Wire.readBytes((byte* )&data, sizeof(data));
                  Serial.println(data.x);
                  Serial.println(data.y);
                  Serial.println(data.z);
                  String aux = "Request: " + String(inputChar) + " - Receiving: ";
                  String(aux).toCharArray(string, 256);
                  serialLib.print(50, 10, string);
                   aux = "X: " + String(data.x) + "Y: " + String(data.y) + "Z: " + String(data.z);
                  String(aux).toCharArray(string, 256);
                  serialLib.print(70, 10, string);                  
                  break;
        default:
                return;
                  
      }
    }
    
  }
}
