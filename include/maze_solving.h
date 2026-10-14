#pragma once
#include "common.h"
#include "action.h"

void mazeSolving();
void mazeSolvingLeft();
void mazeSolvingRight();

LinearMoveByTimer moveStraight3cm(150);
LinearMoveByTimer moveStraight5cm(250);
LinearMoveByTimer moveStraight6cm(300);
LinearMoveByTimer moveBack3cm(150, false);
LinearMoveByTimer moveBack10cm(500, false);

void mazeSolving() {
  mazeSolvingLeft();
}

int freeLeft = 0;
int freeRight = 0;
int lastDistanceFront = 0;
int stuckCounter = 0;
bool needToMoveForward = false;

void mazeSolvingLeft() {
  int distanceLeft = preciseDistanceOn(LEFT);
  int distanceFront = preciseDistanceOn(FRONT);

  if (distanceLeft > 30) {
    if (needToMoveForward) {
      blockingPerform(&turnRight);
      blockingPerform(&moveStraight3cm);
      blockingPerform(&turnLeft);
      blockingPerform(&moveStraight3cm);
    }
    if (freeLeft > 0) {
      blockingPerform(&turnLeft);
      blockingPerform(&moveStraight5cm);
      freeLeft = 0;
      needToMoveForward = true;
    } else {
      freeLeft++;
    }
  } else if (distanceFront < 10) {
    blockingPerform(&turnRight);
    needToMoveForward = false;
    return;
  } else {
    needToMoveForward = false;
  }
  if (abs(lastDistanceFront - distanceFront) < 2) {
    stuckCounter++;
  } else {
    stuckCounter = 0;
  }
  if (stuckCounter > 2) {
    blockingPerform(&moveBack3cm);
    blockingPerform(&turnLeft);
    blockingPerform(&moveStraight5cm);
    blockingPerform(&turnRight);

    stuckCounter = 0;
  }
  blockingPerform(&moveStraight3cm);
  
  lastDistanceFront = distanceFront;
}

void mazeSolvingRight() {
  if (preciseDistanceOn(RIGHT) > 30) {
    PRINT("\nTURNING RIGHT\n");
    blockingPerform(&moveStraight6cm);
    blockingPerform(&turnRight);
    blockingPerform(&moveStraight5cm);
  } else if (preciseDistanceOn(FRONT) < 10) {
    PRINT("\nTURNING RIGHT\n");
    blockingPerform(&turnLeft);
  } else {
    PRINT("\nMOVING FORWARD\n");
    blockingPerform(&moveStraight3cm);
  }
}