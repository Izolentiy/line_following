// #include "line_following.h"
// #include "maze_solving.h"
#include "garbage_collection.h"

void setup() {
  Serial.begin(9600);

  setupGyroscope();
  setupMotors();
  setupServo();
}

void loop() {
  // lineFollowing();
  // mazeSolving();
  garbageCollection();
}
