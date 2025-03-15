#include <Wire.h>
#include "UnoJoy.h"

const uint8_t TOP_SENSOR_PIN = A0;
const uint8_t BOTTOM_SENSOR_PIN = A1;
const int COUNTER_CLOCKWISE = -1;
const int CLOCKWISE = 1;
const int ANGLE_INCREMENT = 3;
const int MAX_ANGLE = 255;
const int MIN_ANGLE = 0;
const int CENTER_ANGLE = 128;

int lastTopSensorState = 0;
int lastBottomSensorState = 0;
int angle = CENTER_ANGLE;
int direction = 0;

void setup() {
  pinMode(TOP_SENSOR_PIN, INPUT);
  pinMode(BOTTOM_SENSOR_PIN, INPUT);
  setupUnoJoy();
  setInitialState();
//  Serial.begin(9600);
}

void setInitialState() {
  lastTopSensorState = getSensorState(TOP_SENSOR_PIN);
  lastBottomSensorState = getSensorState(BOTTOM_SENSOR_PIN);
}

void loop() {
  delay(10);
  int topSensorState = getSensorState(TOP_SENSOR_PIN);
  int bottomSensorState = getSensorState(BOTTOM_SENSOR_PIN);

  dataForController_t controllerData = getControllerData();
  setControllerData(controllerData);
  
  if (noChange(topSensorState, bottomSensorState)) {
    return;
  }

  direction = getWheelDirection(topSensorState, bottomSensorState);
  angle = getWheelAngle(direction);

  lastTopSensorState = topSensorState;
  lastBottomSensorState = bottomSensorState;

  // Serial.print("angle:");
  // Serial.println(angle);
}

int getWheelDirection(int topSensorState, int bottomSensorState) {
  if((topSensorState == 1 && bottomSensorState == 0) && (lastTopSensorState == 0 && lastBottomSensorState == 0)) {
    return COUNTER_CLOCKWISE;
  }

  if((topSensorState == 1 && bottomSensorState == 0) && (lastTopSensorState == 1 && lastBottomSensorState == 1)) {
    return CLOCKWISE;
  }

  if((topSensorState == 0 && bottomSensorState == 0) && (lastTopSensorState == 0 && lastBottomSensorState == 1)) {
    return COUNTER_CLOCKWISE;
  }

  if((topSensorState == 0 && bottomSensorState == 0) && (lastTopSensorState == 1 && lastBottomSensorState == 0)) {
    return CLOCKWISE;
  }

  if((topSensorState == 0 && bottomSensorState == 1) && (lastTopSensorState == 0 && lastBottomSensorState == 0)) {
    return CLOCKWISE;
  }

  if((topSensorState == 0 && bottomSensorState == 1) && (lastTopSensorState == 1 && lastBottomSensorState == 1)) {
    return COUNTER_CLOCKWISE;
  }

  if((topSensorState == 1 && bottomSensorState == 1) && (lastTopSensorState == 1 && lastBottomSensorState == 0)) {
    return COUNTER_CLOCKWISE;
  }

  if((topSensorState == 1 && bottomSensorState == 1) && (lastTopSensorState == 0 && lastBottomSensorState == 1)) {
    return CLOCKWISE;
  }
}

int getWheelAngle(int wheelTurnDirection) {
  int newAngle = angle;
  
  if (wheelTurnDirection == CLOCKWISE) {
    newAngle = angle - ANGLE_INCREMENT;
  }

  if(direction == COUNTER_CLOCKWISE) {
    newAngle = angle + ANGLE_INCREMENT;
  }

  if (newAngle <= MIN_ANGLE) {
    newAngle = MIN_ANGLE;
  }

  if (newAngle >= MAX_ANGLE) {
    newAngle = MAX_ANGLE;
  }
  return newAngle;
}

int toSensorState(int value) {
  if (value <= 300) {
    return false;
  } else {
    return true;
  }
}

int getSensorState(uint8_t sensorPin) {
  int sensorValue = analogRead(sensorPin);

  return toSensorState(sensorValue);
}

bool noChange(int topSensorState, int bottomSensorState) {
  return topSensorState == lastTopSensorState && bottomSensorState == lastBottomSensorState;
}

dataForController_t getControllerData(void){
  dataForController_t controllerData = getBlankDataForController();
  controllerData.leftStickX = angle; // map(constrain(angle, MIN_ANGLE, MAX_ANGLE), MIN_ANGLE, MAX_ANGLE, 52, 200);
  controllerData.leftStickY = 128;

  return controllerData;
}
