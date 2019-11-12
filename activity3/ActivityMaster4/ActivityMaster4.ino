/*
 * MASTER CONFIG
 */
#include "DHT.h"
#include "Wire.h"
//#include "MyLibrary.h"
#include "SoftwareSerial.h"
#define SLAVE_ADDR 0x30

// SoftwareSerial sf(8, 9); //rx rt
// MyLib serialLib =  MyLib();
SoftwareSerial xbee(2,3);

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
  Serial.begin(115200); 
  xbee.begin(9600);
   Wire.begin();
   writeTo(DEVICE_ADDRESS, DATA_FORMAT, 0x01); //Poner ADXL345 en +- 4G
   writeTo(DEVICE_ADDRESS, POWER_CTL, 0x08);  //Poner el ADXL345  
}

void loop() {

    float x, y, z;
    readAccel(x, y, z);
    int posX, posY;

    data.x = x;
    data.y = y;
    data.z = z;

  //aux = "Distance: " + atoi(xbee.read());
  //String(aux).toCharArray(string, 256);
  Serial.write((unsigned char)0x7C);
  Serial.write((unsigned char)0x00);

  printToScreen(10,60);
  Serial.write((char*)"Distance: ");
  xbee.print('d'); 
  receiveDataSlave();
  
  printToScreen(10,50);
  Serial.write((char*) "Temperature: ");
  xbee.print('t');
  receiveDataSlave();
  
  printToScreen(10,40);
  Serial.write((char*) "Humidity: ");
  xbee.print('h');
  receiveDataSlave();

  printToScreen(10, 30);
  Serial.write((char*) "Accelerometer:");
  printToScreen(10, 20);
  Serial.print("x: ");
  Serial.print(x);
  Serial.print(" y: ");
  Serial.print(y);
  printToScreen(10, 10);
  Serial.print("z: ");
  Serial.print(z);

  delay(5000);

}

void readAccel(float &x, float &y, float &z) {
  //Leer los datos
  
  uint8_t numBytesToRead = 6;
  readFrom(DEVICE_ADDRESS, DATAX0, numBytesToRead, _buff);

  //Leer los valores del registro y convertir a int (Cada eje tiene 10 bits, en 2 Bytes LSB)
  x = (((int)_buff[1]) << 8) | _buff[0];   
  y = (((int)_buff[3]) << 8) | _buff[2];
  z = (((int)_buff[5]) << 8) | _buff[4];
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

void printToScreen(int posX, int posY){
  
  Serial.write((unsigned char)(0x7C));
  Serial.write((unsigned char)(0x18));
  Serial.write((unsigned char)(posX));

  delay(10);

  Serial.write((unsigned char)(0x7C));
  Serial.write((unsigned char)(0x19));
  Serial.write((unsigned char)(posY));

 delay(10);
}

void receiveDataSlave(){
  char asd;
  while(!xbee.available());
  do{
      if(xbee.available()){
        asd = xbee.read();
        if(asd != '*')
          Serial.print(asd); 
      } 
    }while(asd != '*');
 
  delay(10);
}
