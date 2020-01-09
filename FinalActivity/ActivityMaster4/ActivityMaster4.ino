#include "Wire.h"
#include "SoftwareSerial.h"
#define SLAVE_ADDR 0x30

SoftwareSerial xbee(2,3);

char slaveRequest;
int request;
uint8_t temperature = 0;
uint8_t humidity = 0;
uint8_t distance = 0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); 
  xbee.begin(9600);
  Wire.begin(SLAVE_ADDR);
  Wire.onReceive(receiveFUNC);
  Wire.onRequest(sendFunc);
}

void sendFunc() {
  switch(request)
  {
    case 1:
    {
      Serial.print("TEMP ");
      Serial.println((uint8_t)temperature);
      Wire.write((uint8_t)temperature);
      break;
    }
    case 2:
    {
      Serial.print("HUM ");
      Serial.println((uint8_t)humidity);
      Wire.write((uint8_t)humidity);
      break;
    }
    case 3:
    {
      Serial.print("DIST ");
      Serial.println((uint8_t)distance);
      Wire.write((uint8_t)distance);
      break;
    }
    default:
      break;
  }
}

void receiveFUNC() {
  while(Wire.available()){
    request = Wire.read();
  }
}

void loop() {
  slaveRequest = 't';
  Serial.println("sending t");
  xbee.print('t');
  receiveDataSlave();
  Serial.println("received t");

  slaveRequest = 'd';
  Serial.println("sending d");
  xbee.print('d');
  receiveDataSlave();
  Serial.println("received d");

  slaveRequest = 'h';
  Serial.println("sending h");
  xbee.print('h');
  receiveDataSlave();
  Serial.println("received h");

  delay(5000);
}

void receiveDataSlave(){
  char asd;
  char str[100];
  int i;
  while(!xbee.available());

  i = 0;
  do{
      if(xbee.available()){
        asd = xbee.read();  
        
        if (asd != '*')
          str[i++] = asd;
        
      } 
    }while(asd != '*');

    Serial.println(str);

    switch(slaveRequest)
    {
      case 't':
      {
        temperature = atoi(str);
        Serial.println(temperature);
        break;
      }
      case 'd':
      {
        distance = atoi(str);
        Serial.println(distance);
        break;
      }
      case 'h':
      {
        humidity = atoi(str);
        Serial.println(humidity);
        break;
      }
      default:
        break;
    }
 
  delay(10);
}
