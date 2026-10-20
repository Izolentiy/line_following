// #define MODE_LINE_FOLLOWING
#define MODE_MAZE_SOLVING
// #define MODE_GARBAGE_COLLECTION

#if defined(MODE_LINE_FOLLOWING)
#include "line_following.h"
#elif defined(MODE_MAZE_SOLVING)
#include "maze_solving.h"
#elif defined(MODE_GARBAGE_COLLECTION)
#include "garbage_collection.h"
#endif

void setup() {
  Serial.begin(9600);

  setupGyroscope();
  setupMotors();
  setupServo();
#ifdef MODE_LINE_FOLLOWING
  setupLineSensors();
#endif
}

void loop() {
#if defined(MODE_LINE_FOLLOWING)
  lineFollowing();
#elif defined(MODE_MAZE_SOLVING)
  mazeSolving();
#elif defined(MODE_GARBAGE_COLLECTION)
  garbageCollection();
#endif
}
