/*
 * MASTER CONFIG
 */
#include "DHT.h"
#include "Wire.h"
#include "MyLibrary.h"
#define SLAVE_ADDR 0x30

SoftwareSerial sf(8, 9); //rx rt
MyLib serialLib =  MyLib(sf);

const int DEVICE_ADDRESS = (0x53);  
byte _buff[6];
char POWER_CTL = 0x2D;
char DATA_FORMAT = 0x31;
char DATAX0 = 0x32;  //X-Axis Data 0
char DATAX1 = 0x33; //X-Axis Data 1
char DATAY0 = 0x34; //Y-Axis Data 0
char DATAY1 = 0x35; //Y-Axis Data 1
char DATAZ0 = 0x36; //Z-Axis Data 0
char DATAZ1 = 0x37; //Z-Axis Data 1

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
   writeTo(DEVICE_ADDRESS, DATA_FORMAT, 0x01); //Poner ADXL345 en +- 4G
   writeTo(DEVICE_ADDRESS, POWER_CTL, 0x08);  //Poner el ADXL345  
}

void loop() {

    float x, y, z;
    readAccel(x, y, z);
    data.x = x;
    data.y = y;
    data.z = z;

    delay(500);
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
                  serialLib.print(10, 50, string);                 
                  break;
        case 'd':
                  int distance;
                  Wire.requestFrom(SLAVE_ADDR, sizeof(int));
                  Wire.readBytes((byte* )&distance, sizeof(int));
                  Serial.println(distance);
                  aux = "Request: " + String(inputChar) + " - Receiving: " + String(distance);
                  String(aux).toCharArray(string, 256);
                  serialLib.print(10, 50, string);                  
                  break;
        case 'h':
                  float humidity;
                  Wire.requestFrom(SLAVE_ADDR, sizeof(float));
                  Wire.readBytes((byte* )&humidity, sizeof(float));
                  Serial.println(humidity);
                  aux = "Request: " + String(inputChar) + " - Receiving: " + String(humidity);
                  String(aux).toCharArray(string, 256);
                  serialLib.print(10, 50, string);                  
                  break;
        case 'a': 
                  Serial.println(data.x);
                  Serial.println(data.y);
                  Serial.println(data.z);
                  String aux = "Request: " + String(inputChar) + " - Receiving: ";
                  String(aux).toCharArray(string, 256);
                  serialLib.print(10, 50, string);
                   aux = "X: " + String(data.x) + " Y: " + String(data.y) + " Z: " + String(data.z);
                  String(aux).toCharArray(string, 256);
                  serialLib.print(0, 30, string);                  
                  break;
        default:
                return;
                  
      }
    }
    
  }
}

void readAccel(float &x, float &y, float &z) {
  //Leer los datos
  
  uint8_t numBytesToRead = 6;
  readFrom(DEVICE_ADDRESS, DATAX0, numBytesToRead, _buff);

  //Leer los valores del registro y convertir a int (Cada eje tiene 10 bits, en 2 Bytes LSB)
  x = (((int)_buff[1]) << 8) | _buff[0];   
  y = (((int)_buff[3]) << 8) | _buff[2];
  z = (((int)_buff[5]) << 8) | _buff[4];

  //Wire.write((byte*)&d, sizeof(data));
}

void writeTo(int device, byte address, byte val) 
{
  Wire.beginTransmission(device);
  Wire.write(address);
  Wire.write(val);
  Wire.endTransmission();
}

void readFrom(int device, byte address, int num, byte buff[])
{
  Wire.beginTransmission(device);
  Wire.write(address);
  Wire.endTransmission();

  Wire.beginTransmission(device);
  Wire.requestFrom(device, num);

  int i = 0;
  while(Wire.available())
  {
    buff[i++] = Wire.read();  
  }
  
  Wire.endTransmission();
}
