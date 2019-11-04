#include "SoftwareSerial.h"

#ifndef LIB_H // in order to include n times
#define LIB_H

struct MyLib {
  public:
    SoftwareSerial sf; //rx
    MyLib(SoftwareSerial sf);
    void print(unsigned char x, unsigned char y, char *str);    
    void circle(unsigned char x, unsigned char y, unsigned char radius);
    void line(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2, unsigned char ink);
    void rectangle(unsigned char x, unsigned char y, unsigned char length, unsigned char height, unsigned char ink);
    void sPixel(unsigned char x, unsigned char y);
    void clPixel(unsigned char x, unsigned char y);
    void clearScreen();

};
#endif
