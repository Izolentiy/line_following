#pragma once
#include "common.h"
#include "action.h"

void setupLineSensors();
void moveStraight();
void sharpLeft();
void sharpRight();
void lineFollowing();


void setupLineSensors() {
  pinMode(S_LEFT_MOST, INPUT);
  pinMode(S_LEFT, INPUT);
  pinMode(S_RIGHT, INPUT);
  pinMode(S_RIGHT_MOST, INPUT);
}

void moveStraight() {
  leftSpeed = BASE_SPEED;
  rightSpeed = BASE_SPEED;
}

void sharpLeft() {
  leftSpeed = -TURN_SPEED;
  rightSpeed = TURN_SPEED;
}

void sharpRight() {
  leftSpeed = TURN_SPEED;
  rightSpeed = -TURN_SPEED;
}

void lineFollowing() {
  // Чтение состояния датчиков (LOW = линия обнаружена)
  bool s1 = digitalRead(S_LEFT_MOST) == HIGH;
  bool s2 = digitalRead(S_LEFT) == HIGH;
  bool s3 = digitalRead(S_RIGHT) == HIGH;
  bool s4 = digitalRead(S_RIGHT_MOST) == HIGH;

  if (s2 || s3) {
    // Центр линии - движение прямо
    moveStraight();
  } else if (s4) {
    // Крайняя линия справа - резкий поворот направо
    sharpRight();
  } else if (s1) {
    // Крайняя линия слева - резкий поворот налево
    sharpLeft();
  } else {
    // Линия потеряна
  }
  applyPeriodicSpeeds(75);
}

