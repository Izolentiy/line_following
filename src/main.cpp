// #include "line_following.h"
// #include "garbage_collection.h"
// #include "maze_solving.h"
#include "raw/concepts.h"

void setup() {
  Serial.begin(9600);

  setupGyroscope();
  setupMotors();
  setupServo();
}

enum mode { LINE_FOLLOWING, MAZE_SOLVING, GARBAGE_COLLECTION };
mode currentMode = GARBAGE_COLLECTION;

Action *garbageCollectionSteps[14] = {
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
ComplexAction garbageCollection(garbageCollectionSteps, 14, true);

LinearMoveToDistance moveBack(-30);

void loop() {

  Serial.println("Hello");
  // moveBack.perform();
  // garbageCollection.perform();
  // moveBackwardShort.perform();
  
  // switch (current_mode) {
  // case LINE_FOLLOWING: lineFollowing(); break;
  // case MAZE_SOLVING: mazeSolving(); break;
  // case GARBAGE_COLLECTION: garbageCollection(); break;
  // }
}
