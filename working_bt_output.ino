/*   
HC05 - Bluetooth AT-Command mode  
modified on 10 Feb 2019 
by Saeed Hosseini 
https://electropeak.com/learn/ 
*/ 
#include <SoftwareSerial.h> 
SoftwareSerial MyBlue(2, 3); // RX | TX 
int flag = 0; 
int LED = 7; 
void setup() 
{   
  Serial.begin(9600); 
  MyBlue.begin(9600); 
  pinMode(LED, OUTPUT); 
  digitalWrite(LED, LOW);
  Serial.println("Ready to connect\nDefualt password is 1234 or 000"); 
} 
void loop() 
{ 
  if (MyBlue.available()) 
    flag = MyBlue.read(); 
    char letter = flag;
    if(flag != 10 && flag != 13 && flag != 0){
      Serial.println(letter);
    }
  if (flag == 49) 
  { 
    digitalWrite(LED, HIGH); 
    Serial.println("LED On"); 
  } 
  else if (flag == 48 || flag == 0) 
  { 
    digitalWrite(LED, LOW); 
    Serial.println("LED Off"); 
  } 
  delay(1000);
} 