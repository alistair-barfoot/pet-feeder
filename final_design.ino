#include <SoftwareSerial.h>
#include <Servo.h>

SoftwareSerial BTSerial(2,3); // RX, TX
Servo myservo;

const int trigPin = 7;
const int echoPin = 8;
int flag = 0;

long duration; // variables for the ultrasonic sensor
int distance;
int times;
int closed = true; // start with the food chamber closed
int pos = 0; // set to start pos
String message = "";
String messageBuffer = "";

void setup(){
  Serial.begin(9600); // set up BT module
  BTSerial.begin(9600);
  Serial.println("Bluetooth connected");

  myservo.attach(9); // set up servo
  myservo.write(pos); // initial servo pos

  pinMode(trigPin, OUTPUT); // set up ultrasonic
  pinMode(echoPin, INPUT);
  
}

void loop(){
  if (BTSerial.available()) 
    flag = BTSerial.read(); 
    char letter = flag;
    if(flag != 10 && flag != 13 && flag != 0){ // weird characters
      Serial.println(letter);
      messageBuffer += letter;
      if(flag == 59){ // ;
        message = messageBuffer;
        messageBuffer = "";
        Serial.print(message);
      }
    }
  delay(1000);
}

void openHatch() {
  if(closed == false){
    return;
  }
  for (pos = 0; pos <= 30; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  closed = false;
}

void closeHatch() {
  if(closed == true){
    return;
  }
    for (pos = 30; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  closed = true;
}

int getDist(){ // output in cm
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2;
  // Prints the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  return distance;
}
