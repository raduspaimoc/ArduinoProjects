#include "SoftwareSerial.h"
#include "MyLibrary.h"

SoftwareSerial sf(8, 9); //rx rt
MyLib serialLib =  MyLib(sf);


/*void circle(int x, int y, int radius){
  // , to draw a circle at center (80, 64) with radius 10 send 0x7C 0x03 0x50 0x40 0x0A 0x01
  sf.write((byte)0x7C);
  sf.write((byte)0x03);
  sf.write((byte)x);
  sf.write((byte)y);
  sf.write((byte)radius);
  sf.write((byte)0x01);
}

void line(int x1, int y1, int x2, int y2, int ink){
  sf.write((byte)0x7C);
  sf.write((byte)0x0C);
  sf.write((byte)x1);
  sf.write((byte)y1);
  sf.write((byte)x2);
  sf.write((byte)y2);
  sf.write((byte)ink);
}

void rectangle(int x, int y, int length, int height, int ink){
  // (0,10) to (50,60) send 0x7C 0x0F 0x00 (x1) 0x0A (y1) 0x32 (x2) 0x3C (y2) 0x01,
  sf.write((byte)0x7C);
  sf.write((byte)0x0F);  
  sf.write((byte)x);
  sf.write((byte)y);
  sf.write((byte)(x + height));
  sf.write((byte)(y + length));
  sf.write((byte)ink);
  
}

void print(int x, int y, char *str){
  sf.write((byte)0x7C);
  //sf.write((byte)x);
  //sf.write((byte)y);
  //sf.write((byte)0x10);
  sf.write((byte)0x18);
  sf.write((byte)x);
  sf.write((byte)0x7C);
  sf.write((byte)0x19);
  sf.write((byte)y);
  sf.write(str);
  //sf.write("LA p");
  //sf.write((byte)0x01);
}

void sPixel(int x, int y){
  // to set the pixel at (80, 64) send 0x7C 0x10 0x50 0x40 0x01,
  sf.write((byte)0x7C);
  sf.write((byte)0x10);
  sf.write((byte)x);
  sf.write((byte)y);
  sf.write((byte)0x01); 
}

void clPixel(int x, int y){
  sf.write((byte)0x7C);
  sf.write((byte)0x10);
  sf.write((byte)x);
  sf.write((byte)y);
  sf.write((byte)0x00); 
}*/

void setup() {
  // put your setup code here, to run once:
  //sf.begin(115200);
  //Serial.begin(9600);
  //sf.write(124);
  //sf.write((byte) 0);

  /*Clear page*/
  /*sf.write((byte)0x7C);
  sf.write((byte)0x00);*/
  
  serialLib.circle(50, 30, 10);
  serialLib.line(0, 10, 50, 60, 1);
  serialLib.line(100, 10, 50, 60, 1);
  serialLib.rectangle(0, 10, 50, 100, 1);
  char *string = (char *) "LA PRACTICA";
  //strcpy(string, "PRACTICA 1");
  serialLib.print(2, 5, string);
  serialLib.sPixel(51, 50);
  /*sf.write((byte)0x7C);
  sf.write((byte)0x0C);
  sf.write((byte)0x00);
  sf.write((byte)0x0A);
  sf.write((byte)0x32);
  sf.write((byte)0x3C);
  sf.write((byte)0x01);*/
  /*circle(50, 30, 10);
  line(0, 10, 50, 60, 1);
  line(100, 10, 50, 60, 1);
  rectangle(0, 10, 50, 100, 1);
  char *string = (char *) "LA PRACTICA";
  //strcpy(string, "PRACTICA 1");
  print(50, 50, string);
  sPixel(51, 50);
  //clPixel(0,0);
 
  
 // 0x7C 0x0C 0x00 (x1) 0x0A (y1)
 // 0x32 (x2) 0x3C (y2) 0x01,*/ 

}

void loop() {
  // put your main code here, to run repeatedly:
  delay(2000);
  serialLib.clPixel(51, 50);
  /*sf.write((byte)124);
  sf.write((byte)16);
  sf.write(10);
  sf.write(10);
  sf.write((byte)1);
  delay(2000);*/
  /*delay(2000);
  clPixel(51, 50);*/

}
