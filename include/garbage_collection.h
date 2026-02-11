#pragma once
#include "common.h"
#include "action.h"
#include "maze_solving.h"

void garbageCollection();

LinearMoveToDistance moveStraight11cm(11);
LinearMoveToDistance moveBackward7cm(-7);
LinearMoveToDistance moveForward14cm(14);
CheckSpaceInDirection checkSpaceOnLeft(LEFT);
CheckSpaceInDirection checkSpaceOnRight(RIGHT);

Action *zigzagSteps[14] = {
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
ComplexAction zigzagMovement(zigzagSteps, 14, true);
Action enterArea;
class MoveToExit : public Action {
  private:
    bool shouldStop = false;
    bool rightMazeSolving = false;
    ActionState checkState() override {
      if (shouldStop) return ENDING;
      return IN_PROGRESS;
    }
    void onStart() override {
      currAngle = getAngleZ();
      if (abs(currAngle) > PI_HALF) {
        rightMazeSolving = true;
      }
    }
    void inLoop() override {
      if (rightMazeSolving) {
        if (preciseDistanceOn(RIGHT) > 30) {
          PRINT("\nTURNING RIGHT\n");
          blockingPerform(&moveStraight11cm);
          blockingPerform(&moveBackward7cm);
          blockingPerform(&turnRight);
          blockingPerform(&moveStraight15cm);
        } else if (preciseDistanceOn(FRONT) < 10) {
          PRINT("\nTURNING RIGHT\n");
          blockingPerform(&moveBackward7cm);
          blockingPerform(&turnLeft);
        } else {
          PRINT("\nMOVING FORWARD\n");
          blockingPerform(&moveStraight3cm);
        }
      } else {
        if (preciseDistanceOn(LEFT) > 30) {
          PRINT("\nTURNING LEFT\n");
          blockingPerform(&moveStraight11cm);
          blockingPerform(&moveBackward7cm);
          blockingPerform(&turnLeft);
          blockingPerform(&moveStraight15cm);
        } else if (preciseDistanceOn(FRONT) < 10) {
          PRINT("\nTURNING RIGHT\n");
          blockingPerform(&moveBackward7cm);
          blockingPerform(&turnRight);
        } else {
          PRINT("\nMOVING FORWARD\n");
          blockingPerform(&moveStraight3cm);
        }
      }
    }
  public:
} moveToExit;

Action *collectionSteps[2] = {
  // &enterArea,
  &zigzagMovement,
  &moveToExit
};
ComplexAction collectGarbage(collectionSteps, 2, false);

void garbageCollection() {
  collectGarbage.perform();
}