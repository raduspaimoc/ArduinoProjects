#include "DHT.h"
#include <Wire.h>
 
// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)
 
// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

#define SLAVE_ADDR 0x30

 
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
        Wire.write((byte*)&temperature, sizeof(float));
        break;
      case 'd':
        distance = ping(TriggerPin, EchoPin);
        Wire.write((byte*)&distance, sizeof(int));
        break;
      case 'h':
        Wire.write((byte*)&humidity, sizeof(float));
        break;     
       default:
        return;
    }
}

void receiveFUNC(){
  while(Wire.available())
    ch = (char)Wire.read();
}

 
void setup() {
   Serial.begin(9600);
   pinMode(TriggerPin, OUTPUT);
   pinMode(EchoPin, INPUT);
   
   Wire.begin(SLAVE_ADDR);
   Wire.onReceive(receiveFUNC);
   Wire.onRequest(sendFunc);
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
  delay(500);
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
