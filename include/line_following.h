#pragma once
#include "common.h"

void setupLineSensors() {
  pinMode(S_LEFT_MOST, INPUT);
  pinMode(S_LEFT, INPUT);
  pinMode(S_RIGHT, INPUT);
  pinMode(S_RIGHT_MOST, INPUT);
}

void lineFollowing() {
  bool leftMost = digitalRead(S_LEFT_MOST) == HIGH;
  bool left = digitalRead(S_LEFT) == HIGH;
  bool right = digitalRead(S_RIGHT) == HIGH;
  bool rightMost = digitalRead(S_RIGHT_MOST) == HIGH;

  if (left || right) {
    // линия под центром - прямо
    setMotorSpeeds(BASE_SPEED, BASE_SPEED);
  } else if (rightMost) {
    // крайний правый датчик - резкий поворот направо
    setMotorSpeeds(TURN_SPEED, -TURN_SPEED);
  } else if (leftMost) {
    // крайний левый датчик - резкий поворот налево
    setMotorSpeeds(-TURN_SPEED, TURN_SPEED);
  }
  // иначе линия потеряна - держим прежние скорости
  applyPeriodicSpeeds(75);
}
