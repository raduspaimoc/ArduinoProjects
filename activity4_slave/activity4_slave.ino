#include "DHT.h"
#include "SoftwareSerial.h"
#include <Wire.h>
 
// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

#define SLAVE_ADDR 0x30

SoftwareSerial xbee(2,3);
const int DHTPin = 5;     // what digital pin we're connected to
 
DHT dht(DHTPin, DHTTYPE);
const int EchoPin = 8;
const int TriggerPin = 9;

char ch;
int distance;
float humidity;
float temperature;

void sendFunc()
{
  Serial.println(ch);  
  switch(ch)
  {
      case 't':              
        xbee.print(temperature);
        xbee.print('*');
        break;
      case 'd':                 
        xbee.print(distance );
        xbee.print('*');
        break;
      case 'h':
        xbee.print(humidity);
        xbee.print('*');
        break;     
       default:
        return;
  }
}

void receiveFUNC(){
  while(xbee.available())//xbee.read();
    ch = (char)xbee.read();
}

 
void setup() {
   xbee.begin(9600);
   Serial.begin(9600);
   pinMode(TriggerPin, OUTPUT);
   pinMode(EchoPin, INPUT);   
   dht.begin();
   Wire.begin();   
}
 
void loop() 
{    
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();
  
  if (isnan(humidity ) || isnan(temperature)) 
  {
    Serial.println("Failed to read from DHT sensor!");
    return;
  } 
  distance = ping(TriggerPin, EchoPin); 
  
  while(!xbee.available());
  
  ch = (char)xbee.read();
  sendFunc(); 
}
 
int ping(int TriggerPin, int EchoPin) {
   long duration, distanceCm;
   
   digitalWrite(TriggerPin, LOW);  //para generar un pulso limpio ponemos a LOW 4us
   delayMicroseconds(4);
   digitalWrite(TriggerPin, HIGH);  //generamos Trigger (disparo) de 10us
   delayMicroseconds(10);
   digitalWrite(TriggerPin, LOW);
   
   duration = pulseIn(EchoPin, HIGH);  //medimos el tiempo entre pulsos, en microsegundos
   
   distanceCm = duration * 10 / 292/ 2;   //convertimos a distancia, en cm
   return distanceCm;
}
