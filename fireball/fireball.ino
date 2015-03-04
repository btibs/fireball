#include <Wire.h>
#include <Adafruit_LSM303.h>

// pin assignments
const int accelX = 0;
const int accelY = 1;
const int accelZ = 2;

const int buttonPin = 3;
const int maxDice = 7;

// minimum change in detected angle to trigger an event
float deltaThreshold = 0.2;

// track values
float pitch, roll, yaw, lastPitch, lastRoll, lastYaw;
boolean isRolling;
int nDice;

// button press = 1 2 3 etc dice and cycle thru
// but only when it's not rolling

void setup() {
  pitch = 999;
  roll = 999;
  yaw = 999;
  isRolling = false;
  
  nDice = 1;
  pinMode(buttonPin, INPUT);
  Serial.begin(9600);
  randomSeed(analogRead(0)); // should be a floating pin - change as necessary
  
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
  // (for reference using glove accel, need to recalibrate)
  // zeroes should be the minimum accel value
  // ie, x and y when board is lying flat, z when straight up
  float xZero = 508; // range like 450-600??
  float yZero = 557;
  float zZero = 519;
  float scale = 556; // z value at rest i.e. gravity
  
  // update values
  lastPitch = pitch;
  lastRoll = roll;
  lastYaw = yaw;
  
  pitch = (atan(xAcc/sqrt(pow(yAcc, 2) + pow(zAcc, 2))) * (180/PI));
  roll = (atan(yAcc/sqrt(pow(xAcc, 2) + pow(zAcc, 2))) * (180/PI));
  yaw = (acos(zAcc/sqrt(pow(xAcc, 2) + pow(yAcc, 2) + pow(zAcc, 2))) * (180/PI)); 
  
  // we just need to check if the ball has stopped rolling
  // i.e., the accelerations have stabilized
  // we can do this by keeping track of the deltas between readings
  //Serial.println();
  //Serial.println(abs(pitch-lastPitch));
  //Serial.println(abs(roll-lastRoll));
  //Serial.println(abs(yaw-lastYaw));
  if (abs(pitch-lastPitch) > deltaThreshold || abs(roll-lastRoll) > deltaThreshold || abs(yaw-lastYaw) > deltaThreshold) {
    // we have a roll, generate a new random number
    isRolling = true;
    int n = random(1,(6*nDice)+1);
    Serial.print("ROLLING: ");
    Serial.println(n);
    
    // todo: display code here
    // todo:? delay inversely proportional to roll speed
  } else {
    isRolling = false; // could use this to re-initialize a seq?
    if (bPress == HIGH) {
      // only count button presses when ball is still
      nDice++;
      if (nDice > maxDice)
        nDice = 1;
    }
  }
}

