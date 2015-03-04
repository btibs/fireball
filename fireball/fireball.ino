#include <Wire.h>
#include <Adafruit_LSM303.h>
#include <SoftwareSerial.h>

Adafruit_LSM303 lsm;
SoftwareSerial Serial7Segment(7, 8); // RX, TX
char tempString[10];

// pin assignments
const int accelX = 0;
const int accelY = 1;
const int accelZ = 2;

const int buttonPin = 15;
const int maxDice = 7;

// minimum change in detected angle to trigger an event
float deltaThreshold = 0.2;

// track values
float pitch, roll, yaw, lastPitch, lastRoll, lastYaw;
boolean isRolling;
int nDice;

void setup() {
  pitch = 999;
  roll = 999;
  yaw = 999;
  isRolling = false;
  nDice = 1;
  randomSeed(analogRead(0)); // should be a floating pin - change as necessary
  
  pinMode(buttonPin, INPUT);

  Serial.begin(9600);
  Serial7Segment.begin(9600);
  Serial7Segment.write('v'); // reset display
  Serial7Segment.write(0x7A); // brightness control
  Serial7Segment.write((byte)1); // 0 bright 255 dim
  sprintf(tempString, "%04d", 1234);
  Serial7Segment.print(tempString);
  
  if (!lsm.begin()) {
    Serial.println("Unable to initialize LSM303!");
    while(1);
  }
  else
    Serial.println("LSM303 initialized");
}

void loop() {
  // get current button stat
  int bPress = digitalRead(buttonPin);
  
  // read values from the accel
  lsm.read();
  float xAcc = lsm.accelData.x;
  float yAcc = lsm.accelData.y;
  float zAcc = lsm.accelData.z;
  
  // orientation math
  // To calibrate: x_0 and y_0 when board is lying flat, z_0 when board vertical
  // scale = gravity, i.e. z value when board is flat
  float xZero = 8; // 8-24
  float yZero = 0; //0-16
  float zZero = -44; //-44 - 24
  float scale = -928; // 920-940
  
  // update values
  lastPitch = pitch;
  lastRoll = roll;
  lastYaw = yaw;
  
  pitch = (atan(xAcc/sqrt(pow(yAcc, 2) + pow(zAcc, 2))) * (180/PI));
  roll = (atan(yAcc/sqrt(pow(xAcc, 2) + pow(zAcc, 2))) * (180/PI));
  yaw = (acos(zAcc/sqrt(pow(xAcc, 2) + pow(yAcc, 2) + pow(zAcc, 2))) * (180/PI)); 
  
  // check if the ball has stopped rolling
  if (abs(pitch-lastPitch) > deltaThreshold || abs(roll-lastRoll) > deltaThreshold || abs(yaw-lastYaw) > deltaThreshold) {
    // we have a roll, generate a new random number
    int n = random(1,(6*nDice)+1);
    Serial.print("ROLLING: ");
    Serial.print(abs(pitch-lastPitch));
    Serial.print(" ");
    Serial.print(abs(roll-lastRoll));
    Serial.print(" ");
    Serial.print(abs(yaw-lastYaw));
    Serial.print(" ");
    Serial.println(n);
    
    Serial7Segment.write('v'); // reset display
    sprintf(tempString, " %2d ", n);
    Serial7Segment.print(tempString);
  } else {
    if (bPress == LOW) {
      // only count button presses when ball is still
      nDice++;
      if (nDice > maxDice)
        nDice = 1;
      sprintf(tempString, "d  %1d", nDice);
    }
    Serial7Segment.write('v'); // reset display
    Serial7Segment.print(tempString);
    delay(100);
  }
  delay(100);
}

