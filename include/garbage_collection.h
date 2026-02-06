#pragma once
#include "common.h"

bool shouldTurnLeft = true;
bool rotatingRight = false;
bool rotatingLeft = false;
bool resetAngle = false;
bool missionComplete = false;

bool movingBack = true;
bool movingForward = true;
bool turningComplete = false;

float initAngle = 0.0f, currAngle = 0.0f;

void startRotatingToLeft();
void startRotatingToRight();
void adjustSpeedsToMoveForward();
void adjustSpeedsToMoveBack();
uint16_t distanceOn(int angle);
void garbageCollection();


void startRotatingToLeft() {
  rotatingLeft = true;
  setMotorSpeeds(-TURN_SPEED, TURN_SPEED);
}

void startRotatingToRight() {
  rotatingRight = true;
  setMotorSpeeds(TURN_SPEED, -TURN_SPEED);
}

void adjustSpeedsToMoveForward() {
  float diffAngle = initAngle - currAngle;
  int16_t diffTerm = (int16_t)(diffAngle/PI*TURN_STRENGTH);
  if (diffAngle < 0) {
    setMotorSpeeds(BASE_SPEED + diffTerm, BASE_SPEED);
  } else {
    setMotorSpeeds(BASE_SPEED, BASE_SPEED - diffTerm);
  }
}

void adjustSpeedsToMoveBack() {
  adjustSpeedsToMoveForward();
  setMotorSpeeds(-leftSpeed, -rightSpeed);
}

uint16_t distanceOn(int angle) {
  uint8_t initServoAngle = servo.read();
  servo.write(angle);
  delay(abs(initServoAngle - angle) * SERVO_DELAY_MS_PER_DEGREE);
  uint16_t val = sonar.read();
  PRINT("Distance at angle %f: %d", val);
  return val;
}

uint16_t preciseDistanceOn(int angle) {
  return 0;
}

void turningAround() {
  if (movingBack) {
    if (distanceOn(AHEAD) > 25) {
      movingBack = false;
      if (shouldTurnLeft) {
        startRotatingToLeft();
      } else {
        startRotatingToRight();
      }
      return;
    }
    adjustSpeedsToMoveBack();
  } else if (rotatingRight) {
    if (currAngle < initAngle + PI_HALF-0.1f) return;
    rotatingRight = false;
    initAngle += PI_HALF;
    setMotorSpeeds(0, 0);
  } else if (rotatingLeft) {
    rotatingLeft = false;
    initAngle -= PI_HALF;
    setMotorSpeeds(0, 0);
  } else {
    if (movingForward) {
      
      adjustSpeedsToMoveForward();
    } else {  
      turningComplete = true;
    }
  }
}

void garbageCollection() {
  currAngle = getAngleZ();
  
  applyPeriodicSpeeds(100);
  if (missionComplete) {
    setMotorSpeeds(0, 0);
  } else if (rotatingRight) {
    if (currAngle < initAngle + PI_HALF-0.1f) return;
    rotatingRight = false;
    resetAngle = true;
    setMotorSpeeds(0, 0);
  } else if (rotatingLeft) { 
    if (currAngle > initAngle-PI_HALF+0.1f) return;
    rotatingLeft = false;
    resetAngle = true;
    setMotorSpeeds(0, 0);
  } else if (distanceOn(AHEAD) < 20) {
    setMotorSpeeds(0, 0);
    applyMotorSpeeds();
    if (shouldTurnLeft) {
      if (distanceOn(LEFT) < 25) {
        missionComplete = true;
      } else {
        startRotatingToLeft();
      }
    } else {
      if (distanceOn(RIGHT) < 25) {
        missionComplete = true;
      } else {
        startRotatingToRight();
      }
    }
    shouldTurnLeft = !shouldTurnLeft;
  } else {
    if (resetAngle) {
      if (shouldTurnLeft) {
        initAngle = PI_HALF;
      } else {
        initAngle = -PI_HALF;
      }
      resetAngle = false;
    }
    adjustSpeedsToMoveForward();
  }
}