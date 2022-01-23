// Home Protect script
// Made by Bogdan Barbu
// v 0.1.0

// Libraries
#include <Servo.h>
#include <SoftwareSerial.h>

// Pins positon
int pinMotorLeftBack = 5 ;
int pinMotorLeftFront = 3 ;
int pinMotorRightFront = 11 ;
int pinMotorRightBack = 6 ;
SoftwareSerial GSMChannel(8, 7); // defines the pins 8 and 7 as serial pins (for communication)
int servoPin = 2;
int coPin = A1;
int floodPin = A0;
int gasPin = A2;
int buzzer = 12;

// Constants
int coMaxValue = 400;
int gasMaxValue = 1000;
int corectionVoltageValue = 20;
int trigPin = 9; // TRIG ultrasonic pin
int echoPin = 10; // ECHO ultrasonic pin
int speed = 50; // the speed of the robot
Servo Servo1;
bool isCO = false;
bool isWater = false;
bool isGas = false;
bool batteryLow = false;

// setting up everything
void setup() {
  Serial.begin(9600);

  // GSM pins initialization
  GSMChannel.begin(9600);

  // motors initialization
  pinMode(pinMotorLeftBack, OUTPUT);
  pinMode(pinMotorLeftFront, OUTPUT);
  pinMode(pinMotorRightFront, OUTPUT);
  pinMode(pinMotorRightBack, OUTPUT);

  // sensors initialization
  pinMode(gasPin, INPUT);
  pinMode(floodPin, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // make sure that the buzzer is off
  digitalWrite(buzzer, HIGH);

  // servo motor initialization
  Servo1.attach(servoPin);

  // initialize the servo motor to 0 degrees
  Servo1.write(0);

  delay(1000);
}

// declare the speeds
void setTheSpeed(int lbm, int lfm, int frm, int rbm){
  analogWrite(pinMotorLeftBack, lbm); // lbm = left back motor
  analogWrite(pinMotorLeftFront, lfm); 
  analogWrite(pinMotorRightFront, frm);
  analogWrite(pinMotorRightBack, rbm);
}

// co detection function
void detectCO(){
  // read the value from the sensor
  int coValue = analogRead(coPin);

  // if co2 is detected ...
  if (coValue > coMaxValue)
  {
    // report that is co2
    isCO = true;
    Serial.println("CO DETECTED!");

    // stop the robot where it is
    setTheSpeed(0,0,0,0);
    Servo1.write(0);

    // start the buzzer alarm
    digitalWrite(buzzer, LOW);

    // send the warning SMS
    SendMessage("WARNING! Carbon Monoxide HAS BEEN DETECTED!");

    // start a loop while co still is, the robot to do nothing else
    while (isCO == true){

      // check if is still co
      coValue = analogRead(coPin);

      Serial.println(coValue);
      
      // if is not any more ...
      if (coValue < coMaxValue - 20){
        isCO = false;
        digitalWrite(buzzer, HIGH);
      }
    }
  }
}

// flood detecton function
void detectFlood(){

  // read the value from the sensor
  int waterValue = analogRead(floodPin);

  // if water is detected ...
  if (waterValue <= 500)
  {
    // report flood
    isWater = true;
    Serial.println("Water!");

    // stop the robot where it is
    setTheSpeed(0,0,0,0);
    Servo1.write(0);

    // start the buzzer alarm
    digitalWrite(buzzer, LOW);

    // send the warning SMS
    SendMessage("WARNING! FLOOD HAS BEEN DETECTED!");

    // start a loop while water still is, the robot to do nothing else
    while (isWater == true){

      // check if is still water
      waterValue = analogRead(floodPin);

      // if is not any more ...
      if (waterValue > 520){
        isWater = false;
        digitalWrite(buzzer, HIGH);
      }
    }
  }
}

// gas detection function
void detectGas(){

  // read the value from the sensor
  int gasValue = analogRead(gasPin);

  // if gas is deteced ...
  if (gasValue > gasMaxValue)
  {
    // report gas
    isGas = true;
    Serial.println("GAS DETECTED!");
    
    // stop the robot where it is
    setTheSpeed(0,0,0,0);
    Servo1.write(0);

    // start the buzzer alarm
    digitalWrite(buzzer, LOW);

    // send the warning SMS
    SendMessage("WARNING! GAS HAS BEEN DETECTED!");

    // start a loop while gas still is, the robot to do nothing else
    while (isGas == true){

      // check if is still water
      gasValue = analogRead(gasPin);

      // if is not any more ...
      if (gasValue < gasMaxValue - 20){
        isGas = false;
        digitalWrite(buzzer, HIGH);
      }
    }
  }
}

// the function which sends the sms
void SendMessage(String text){
  GSMChannel.println("AT+CMGF=1"); // set the gsm in text mode
  delay(1000);
  GSMChannel.println("AT+CMGS=\"+40731351437\"\r"); // the number where the sms is send
  delay(1000);
  GSMChannel.println(text); // the text which the message is send
  delay(100);
  GSMChannel.println((char)26); // exit the SendMessage mode
  delay(1000);
}

void scanZone1(){
  Serial.println("Scan zone 1");
  // get the distance from the distance sensor
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  float time1 = pulseIn(echoPin, HIGH);
  float distance = (time1*.0343)/2;
  moveConditions(distance, "left");
}

void scanZone2(){
  Serial.println("Scan zone 2");
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  float time2 = pulseIn(echoPin, HIGH);
  float distance = (time2*.0343)/2;
  moveConditions(distance, "right");
}

void moveConditions(float recievedDistance, String position){
  if (position == "left") {

    if (recievedDistance <= 30) {  
      // turn right (obstacle at left)
      setTheSpeed(0,0,0,0);
      delay(1000);
      setTheSpeed(speed,0,0,speed);
      delay(1300);
      setTheSpeed(0,speed,0,0);
      delay(1000);
      setTheSpeed(0,speed,speed,0);
    }

    else{
      setTheSpeed(0,speed,speed,0);
    }
  }

  else if(position == "right"){

    if (recievedDistance <= 30) {
      // turn left (obstacle at right)
      setTheSpeed(0,0,0,0);
      delay(1000);
      setTheSpeed(speed,0,0,speed);
      delay(3000);
      setTheSpeed(0,0,speed,0);
      delay(1000);
      setTheSpeed(0,speed,speed,0);
    }

    else{
      setTheSpeed(0,speed,speed,0);
    }
  }
}

void moveServoAndScan(){
  // move the servo motor to scan scan
  Servo1.write(40);
  delay(1000);
  scanZone1();
  Servo1.write(140);
  delay(1000);
  scanZone2();
}

void loop() {
  moveServoAndScan();
  detectCO();
  detectFlood();
  detectGas();
}
