#include <Servo.h>
#include <SoftwareSerial.h>

Servo myservo;  // create servo object to control a servo
SoftwareSerial MySerial(2, 3); // RX | TX 
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position
int closed = true; 
int flag = 0; 


// defines pins numbers
const int trigPin = 7;
const int echoPin = 8;
// defines variables
long duration;
int distance;
int times;

void setup() {
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  Serial.begin(9600);
  MySerial.begin(9600);
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
  myservo.write(0);
  Serial.println("1. Servo");
  Serial.println("2. Ultrasonic");
  Serial.println("3. Bluetooth");
  Serial.println("4. Load");
}

void loop() {
  delay(100);
  Serial.println("Which sensor would you like to read? ");

  while (Serial.available() == 0) {
  }

  int choice = Serial.parseInt();
  if(choice == 1){
    openHatch();
    delay(1000);
    closeHatch();
  } else if(choice == 2){
    for(int i = 0; i < 100; i++){
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
      if(distance <= 10){
        times = times + 1;
      } else {
        times = 0;
      }
      if(times == 5){
        Serial.println("Your pet is by the pet feeder");
      }
    }
  } else if (choice == 3){ // bluetooth
    Serial.println("Waiting for Bluetooth message....");
    while(1){
    if(MySerial.available()){
      flag = MySerial.read();
      char letter = flag;
      if(flag != 10 && flag != 13 && flag != 0 && letter != ';'){
      Serial.print(letter);
      }
      if(letter == ';'){
        break;
        }
    }
   }
   Serial.print('\n');
  }
}

void openHatch() {
  if(closed == false){
    return;
  }
  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
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
    for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  closed = true;
}
