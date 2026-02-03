#pragma once
#include "common.h"

void turnLeft();
void turnRight();
void moveStraight();
bool checkLeft();
bool checkRight();
void garbageCollection();

bool checkLeft() {
  return false;
}

bool checkRight() {
  return false;
}

void garbageCollection() {
  static bool obstacleAhead;
  static bool shouldTurnLeft;
  moveStraight();
  if (obstacleAhead) {
    if (shouldTurnLeft) {
      bool obstacleLeft = checkLeft();
      if (obstacleLeft) {
        // mission complete return to exit
      } else {
        turnLeft();
      }
    } else {
      bool obstacleRight = checkRight();
      if (obstacleRight) {
        // mission complete return to exit
      } else {
        turnRight();
      }
    }
  }


  static float initAngle = getAngleZ();
  float currAngle = getAngleZ();
  float diffAngle = initAngle - currAngle;
  int16_t diffTerm = (int16_t)(diffAngle/3.14*180*8);
  int16_t leftSpeed, rightSpeed;
  if (diffAngle < 0) {
    rightSpeed = MAX_SPEED;
    leftSpeed = MAX_SPEED + diffTerm;
  } else {
    rightSpeed = MAX_SPEED - diffTerm;
    leftSpeed = MAX_SPEED;
  }

  setPeriodicSpeeds(leftSpeed, rightSpeed, 100);

  Serial.println(leftSpeed);
  Serial.println(rightSpeed);
  Serial.println();
  servo.write(90 - (int)(diffAngle/3.14*180));
}