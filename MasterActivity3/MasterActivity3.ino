/*
 * MASTER CONFIG
 */

#include "Wire.h"

#define SLAVE_ADDR 0x30

char inputChar;
uint16_t cal16;
uint8_t cal8;

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

      Wire.beginTransmission(SLAVE_ADDR);
      Wire.write(inputChar);
      Wire.endTransmission();

      switch(inputChar){
        case 't':
                  float t;
                  Wire.requestFrom(SLAVE_ADDR, sizeof(float));
                  Wire.readBytes((byte* )&t, sizeof(float));
                  Serial.println(t);
                  break;
        case 'd':
                  int distance;
                  Wire.requestFrom(SLAVE_ADDR, sizeof(int));
                  Wire.readBytes((byte* )&distance, sizeof(int));
                  Serial.println(distance);
                  break;
        case 'h':
                  float humidity;
                  Wire.requestFrom(SLAVE_ADDR, sizeof(float));
                  Wire.readBytes((byte* )&humidity, sizeof(float));
                  Serial.println(humidity);
                  break;
        case 'a': 
                  Wire.requestFrom(SLAVE_ADDR, sizeof(data));
                  Wire.readBytes((byte* )&data, sizeof(data));
                  Serial.println(data.x);
                  Serial.println(data.y);
                  Serial.println(data.z);
                  break;
        default:
                return;
                  
      }
    }
    
  }
}
