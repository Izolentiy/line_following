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
mode current_mode = GARBAGE_COLLECTION;

void loop() {

  turnLeft.perform();

  // float angle = (getAngleZ() / PI * 180.0f);
  // servo.write(90 + (int)angle);
  // Serial.println(angle);

  // currAngle = getAngleZ();
  // adjustSpeedsToMoveStraight();
  // applyMotorSpeeds();

  // static uint32_t timer = millis();
  // if (millis() - timer > 10000) {
  //   timer = 0;
  //   mpu.dmpInitialize();
  //   mpu.setDMPEnabled(true);
  // }

  // switch (current_mode) {
  // case LINE_FOLLOWING: lineFollowing(); break;
  // case MAZE_SOLVING: mazeSolving(); break;
  // case GARBAGE_COLLECTION: garbageCollection(); break;
  // }
}
