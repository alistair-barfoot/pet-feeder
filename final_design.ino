#include <SoftwareSerial.h>
#include <Servo.h>
#include "HX711.h"

SoftwareSerial BTSerial(2,3); // RX, TX
Servo myservo;
HX711 scale;

const int trigPin = 7;
const int echoPin = 8;
const int LOADCELL_DOUT_PIN = 11;
const int LOADCELL_SCK_PIN = 10;
int flag = 0;
const int bowlWeight = 121;
int counter = 0;
int nextNotif = 0;
int load = 200;

int times[] = {0,0,0,0,0,0,0,0,0,0};
const int hours = 19;
const int minutes = 25;
const int initial_time = 1000*3600*(hours+minutes/60);
int curr_time;
long duration; // variables for the ultrasonic sensor
int distance;
int closed = true; // start with the food chamber closed
int pos = 0; // set to start pos
String message = "";
String messageBuffer = "";

void setup(){
  Serial.begin(9600); // set up BT module
  BTSerial.begin(9600);
  Serial.println("Bluetooth connected");
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale(-292.511);
  scale.tare();

  myservo.write(pos); // initial servo pos

  pinMode(trigPin, OUTPUT); // set up ultrasonic
  pinMode(echoPin, INPUT);
  
}

void loop(){
  curr_time = initial_time + millis();
  for(int i = 0; i < 9; i++){
    if(times[i] == curr_time){
      dispenseFood(load);
    }
  }
  if (BTSerial.available()) {
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
  }
  if(message == "open;"){
    dispenseFood(load);
  } else if (message == "add;"){
    if(times[9] == 0){
      // error : FULL
    }
    BTSerial.print("Please enter the hour of the time");
    if (BTSerial.available()) {
      flag = BTSerial.read(); 
      if(flag != 10 && flag != 13 && flag != 0){ // weird characters
        flag = flag - '0';
      }
    }
    int new_time = flag * 1000 * 3600;
    int arr_pos;
    for(int i = 0; i < 9; i++){
      if(new_time >= times[i] && new_time <= times[i+1]){
        arr_pos = i;
        break;
      }
      arr_pos = 9;
    }
    for(int i = 8; i >= arr_pos; i--){
      times[i+1] = times[i];
    }
    times[arr_pos] = new_time;
  } else if(message == "adjust;"){
    BTSerial.print("Enter the new load");
    if (BTSerial.available()) {
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
  }
    load = strtoint(message);
  }
  int dist = getDist(); // activate the ultrasonic sensor
  if(dist < 10){
    counter = counter + 1;
    if(counter >= 5 && millis() > nextNotif){
      BTSerial.print("Your pet is by the pet feeder");
      nextNotif = millis() + 10 * 60 * 1000;
      counter = 0;
    }
  }
  delay(1000);
}

void dispenseFood(int desiredWeight){
  int scale_reading = scale.read_average(20);
  int weight = scale_reading - bowlWeight;
  do{
    openHatch();
    delay(100);
    closeHatch();
    scale_reading = scale.read_average(20);
    weight = scale_reading - bowlWeight;
  } while(weight < desiredWeight - 50);
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

int strtoint(String input){
  int end;
  int i = 0;
  int output = 0;
  while(input[i] != ';'){
    i++;
  }
  end = i-1;
  int count = 0;
  for(int i = end; i>0; i--){
    output = output + pow(10,count) * (input[i] - '0'); 
    count++;
  }

  return output;
}
