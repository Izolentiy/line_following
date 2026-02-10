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


void startRotatingToLeft();
void startRotatingToRight();
void garbageCollection();


void startRotatingToLeft() {
  rotatingLeft = true;
  setMotorSpeeds(-TURN_SPEED, TURN_SPEED);
}

void startRotatingToRight() {
  rotatingRight = true;
  setMotorSpeeds(TURN_SPEED, -TURN_SPEED);
}

void turningAround() {
  if (movingBack) {
    if (distanceOn(FRONT) > 25) {
      movingBack = false;
      if (shouldTurnLeft) {
        startRotatingToLeft();
      } else {
        startRotatingToRight();
      }
      return;
    }
    adjustSpeedsToMoveBackward();
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
  } else if (distanceOn(FRONT) < 20) {
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