#include "SoftwareSerial.h"
#include "MyLibrary.h"

typedef unsigned char BYTE;

MyLib::MyLib(SoftwareSerial serial) : sf(serial)
{
  sf.begin(115200);
  sf.write((unsigned char)0x7C);
  sf.write((unsigned char)0x00);
};

void MyLib::clearScreen(){
  sf.write((unsigned char)0x7C);
  sf.write((unsigned char)0x00);
}

void MyLib::circle(unsigned char x, unsigned char y, unsigned char radius){
  // , to draw a circle at center (80, 64) with radius 10 send 0x7C 0x03 0x50 0x40 0x0A 0x01
  sf.write((unsigned char)(0x7C));
  sf.write((unsigned char)(0x03));
  sf.write((unsigned char)(x));
  sf.write((unsigned char)(y));
  sf.write((unsigned char)(radius));
  sf.write((unsigned char)(0x01));
}

void MyLib::line(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2, unsigned char ink){
  sf.write((unsigned char)(0x7C));
  sf.write((unsigned char)(0x0C));
  sf.write((unsigned char)(x1));
  sf.write((unsigned char)(y1));
  sf.write((unsigned char)(x2));
  sf.write((unsigned char)(y2));
  sf.write((unsigned char)(ink));
}

void MyLib::rectangle(unsigned char x, unsigned char y, unsigned char length, unsigned char height, unsigned char ink){
  // (0,10) to (50,60) send 0x7C 0x0F 0x00 (x1) 0x0A (y1) 0x32 (x2) 0x3C (y2) 0x01,
  sf.write((unsigned char)(0x7C));
  sf.write((unsigned char)(0x0F));
  sf.write((unsigned char)(x));
  sf.write((unsigned char)(y));
  sf.write((unsigned char)((x + height)));
  sf.write((unsigned char)((y + length)));
  if(ink == 0)
    sf.write((unsigned char)(ink));

}

void MyLib::print(unsigned char x, unsigned char y, char *str){
  sf.write((unsigned char)(0x7C));
  sf.write((unsigned char)(0x18));
  sf.write((unsigned char)(x));

  sf.write((unsigned char)(0x7C));
  sf.write((unsigned char)(0x19));
  sf.write((unsigned char)(y));
  sf.write((char*)str);
}

void MyLib::sPixel(unsigned char x, unsigned char y){
  // to set the pixel at (80, 64) send 0x7C 0x10 0x50 0x40 0x01,
  sf.write((unsigned char)(0x7C));
  sf.write((unsigned char)(0x10));
  sf.write((unsigned char)(x));
  sf.write((unsigned char)(y));
  sf.write((unsigned char)(0x01));
}

void MyLib::clPixel(unsigned char x, unsigned char y){
  sf.write((unsigned char)(0x7C));
  sf.write((unsigned char)(0x10));
  sf.write((unsigned char)(x));
  sf.write((unsigned char)(y));
  sf.write((unsigned char)(0x00));
}
