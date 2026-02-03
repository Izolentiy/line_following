#include "line_following.h"
#include "garbage_collection.h"
#include "maze_solving.h"

void setup() {
  Serial.begin(9600);

  setupGyroscope();
  setupMotors();
  setupServo();
}

enum mode { LINE_FOLLOWING, MAZE_SOLVING, GARBAGE_COLLECTION };
mode current_mode = GARBAGE_COLLECTION;

void loop() {
  switch (current_mode) {
  case LINE_FOLLOWING: lineFollowing(); break;
  case MAZE_SOLVING: mazeSolving(); break;
  case GARBAGE_COLLECTION: garbageCollection(); break;
  }
}
