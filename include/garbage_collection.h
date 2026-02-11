#pragma once
#include "common.h"
#include "action.h"

void garbageCollection();

Action *zigzagSteps[14] = {
  &moveForwardTillWall,
  &checkSpaceOnLeft,
  
  &moveBackwardShort,
  &turnLeft,
  &moveForwardShort,
  &moveBackwardShort,
  &turnLeft,

  &moveForwardTillWall,
  &checkSpaceOnRight,

  &moveBackwardShort,
  &turnRight,
  &moveForwardShort,
  &moveBackwardShort,
  &turnRight
};
ComplexAction zigzagMovement(zigzagSteps, 14, true);
Action enterArea;
Action moveToExit;

Action *collectionSteps[3] = {
  &enterArea,
  &zigzagMovement,
  &moveToExit
};
ComplexAction collectGarbage(collectionSteps, 3, false);

void garbageCollection() {
  collectGarbage.perform();
}