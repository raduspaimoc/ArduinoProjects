#include "MyLibrary.h"

SoftwareSerial sf(8, 9); //rx rt
MyLib serialLib =  MyLib(sf);
int ran;
int num;
int ends = -1;
int starts = -1;
int len = 0;
bool finished = false;
bool game = true;
char *string;
char *aux;
char *aux2;

/*void clearScreen(){
  sf.write((byte)0x7);
  sf.write((byte)0x00);   
}*/
void drawRectangle(){
  serialLib.rectangle(0,0,63,127, 1);  
}

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(0));
  ran = random(1, 1000);
  num = ran;

  drawRectangle();
  string = (char *) "New game started.";  
  serialLib.print(15, 50, string);
  string = (char *) "Press h to get help.";  
  serialLib.print(7, 30, string);

  Serial.println(ran);

  while(num > 0)
  {
      ++len;
      int mod = num % 10;
  
      num = num / 10;

      if (starts == -1)
        ends = mod;

      starts = mod;
  }
}

void loop() 
{  
  if (!Serial.available() || !game)
    return;

  if (finished)
  {
    char c = Serial.read();
    
    if (c == 'y' || c == 'Y')
    {      
      serialLib.clearScreen();
      drawRectangle();
      char *string = (char *) "New game started";  
      serialLib.print(20, 50, string);
      ran = random(1, 1000); 
      Serial.println(ran);

      finished = false;
      game = true;
      return;
    }
    else if (c == 'n' || c == 'N')
    {
      serialLib.clearScreen();
      drawRectangle();
      char *string = (char *) "bye";  
      serialLib.print(20, 50, string);
      Serial.println("bye");
      game = false;
      return;
    }
    else
      return;
  }

  if (Serial.peek() == 'h') // User wants some help
  {
     Serial.println(Serial.readString());  
     Serial.flush();
     
     switch(random(4))
     {
        case 0:
          Serial.print("Starts with: ");
          Serial.println(starts);
          serialLib.clearScreen();
          drawRectangle();
          aux= (char *) "Starts with: ";  
          sprintf(string, "%s %x", aux, starts);
          serialLib.print(10, 50, string);         
          break;
        case 1:
          serialLib.clearScreen();
          drawRectangle();
          aux= (char *) "Ends with: ";
          sprintf(string, "%s %x", aux, ends);     
          serialLib.print(10, 50, string);             
          Serial.print("Ends with: ");
          Serial.println(ends);
          break;
        case 2:
          serialLib.clearScreen();
          drawRectangle();
          aux = (char *) "Has ";  
          aux2 = (char *) " numbers";       
          sprintf(string, "%s %x %s", aux, len, aux2);            
          serialLib.print(10, 50, string);   
          Serial.print("Has ");
          Serial.print(len);
          Serial.println(" numbers");
          break;
        case 3:
          if (ran % 2){
            serialLib.clearScreen();
            drawRectangle();
            string = (char *) "The number is odd";  
            serialLib.print(10, 50, string);
            Serial.println("The number is odd");
          } else {
            serialLib.clearScreen();
            drawRectangle();
            string = (char *) "The number is even";  
            serialLib.print(10, 50, string);
            Serial.println("The number is even"); 
          }
          break;
        default:
          break;
     }
    return;
  }
  
  
  int input = Serial.parseInt();  
  Serial.flush();

  if (ran == input)
  {
    serialLib.clearScreen();
    drawRectangle();
    finished = true;
    string = (char *) "You got it fam.";  
    serialLib.print(20, 30, string);
    string = (char *) "Do you fancy";  
    serialLib.print(5, 20, string);
    string = (char *) "another game? (y/n)";  
    serialLib.print(5, 10, string);
    sprintf(string, "%d", ran);  
    serialLib.print(54, 50, string);
    serialLib.circle(60, 45, 15);
    Serial.println("You got it fam");
    Serial.println("Do you fancy another game? (y/n)");
    return;
  }
  
  if (input > ran){
    serialLib.clearScreen();
    drawRectangle();
    string = (char *) "Is too big";  
    serialLib.print(20, 50, string);
    Serial.flush();
  } else {
    serialLib.clearScreen();
    drawRectangle();
    string = (char *) "Is too small";  
    serialLib.print(20, 50, string); 
    Serial.flush();
    //Serial.println("The number you introduced was too small");
  }

}
