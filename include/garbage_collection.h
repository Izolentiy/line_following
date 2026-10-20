#pragma once
#include "action.h"

LinearMoveToDistance moveStraight11cm(11);
LinearMoveToDistance moveBackward7cm(-7);
LinearMoveToDistance moveForward14cm(14);
CheckSpaceInDirection checkSpaceOnLeft(LEFT);
CheckSpaceInDirection checkSpaceOnRight(RIGHT);

// zigzag across the area: wall -> shift to the left -> wall -> shift to the right
Action *zigzagSteps[] = {
  &moveForwardTillWall,
  &checkSpaceOnLeft,

  &moveBackward7cm,
  &turnLeft,
  &moveForward14cm,
  &moveBackward7cm,
  &turnLeft,

  &moveForwardTillWall,
  &checkSpaceOnRight,

  &moveBackward7cm,
  &turnRight,
  &moveForward14cm,
  &moveBackward7cm,
  &turnRight
};
ComplexAction zigzagMovement(zigzagSteps, sizeof(zigzagSteps) / sizeof(zigzagSteps[0]), true);

// wall-following step: `wallSide` is the side the wall is kept on,
// `toWall` turns to that side and `fromWall` turns away from it
static void followWallStep(int wallSide, Action *toWall, Action *fromWall) {
  if (preciseDistanceOn(wallSide) > 30) {
    PRINT("\nTURNING TO WALL\n");
    blockingPerform(&moveStraight11cm);
    blockingPerform(&moveBackward7cm);
    blockingPerform(toWall);
    blockingPerform(&moveStraight5cm);
  } else if (preciseDistanceOn(FRONT) < 10) {
    PRINT("\nTURNING FROM WALL\n");
    blockingPerform(&moveBackward7cm);
    blockingPerform(fromWall);
  } else {
    PRINT("\nMOVING FORWARD\n");
    blockingPerform(&moveStraight3cm);
  }
}

class MoveToExit : public Action {
  private:
    bool rightMazeSolving = false;
    void onStart() override {
      currAngle = getAngleZ();
      // turned around after the zigzag - the exit is on the other side
      rightMazeSolving = abs(currAngle) > PI_HALF;
    }
    void inLoop() override {
      if (rightMazeSolving) {
        followWallStep(RIGHT, &turnRight, &turnLeft);
      } else {
        followWallStep(LEFT, &turnLeft, &turnRight);
      }
    }
} moveToExit;

Action *collectionSteps[] = {
  &zigzagMovement,
  &moveToExit
};
ComplexAction collectGarbage(collectionSteps, sizeof(collectionSteps) / sizeof(collectionSteps[0]), false);

void garbageCollection() {
  collectGarbage.perform();
}
