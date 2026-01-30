#pragma once
#include <Arduino.h>

#define L_MOTOR_IN1 5  // D5 (ШИМ)
#define L_MOTOR_IN2 7  // D7
#define R_MOTOR_IN1 6  // D6 (ШИМ)
#define R_MOTOR_IN2 8  // D8

void setupMotors();
void stopMotors();

void setLeftMotor(int speed);
void setRightMotor(int speed);

void moveStraight();
void sharpLeft();
void sharpRight();
void turnLeft();
void turnRight();
void correctLeft();
void correctRight();
void staticLeft();
void staticRight();

float getAngle();

float startAngle = 0.0f;


#define BASE_SPEED 180  // Базовая скорость 180
#define MIN_SPEED 120   // Минимальная скорость 80
#define TURN_FACTOR 50  // Сила поворота 90

void setupMotors() {
  pinMode(L_MOTOR_IN1, OUTPUT);
  pinMode(L_MOTOR_IN2, OUTPUT);
  pinMode(R_MOTOR_IN1, OUTPUT);
  pinMode(R_MOTOR_IN2, OUTPUT);
}

void setLeftMotor(int speed) {
  if (speed >= 0) {
    analogWrite(L_MOTOR_IN1, speed);
    digitalWrite(L_MOTOR_IN2, LOW);
  } else {
    analogWrite(L_MOTOR_IN1, speed);
    digitalWrite(L_MOTOR_IN2, HIGH);
  }
}

void setRightMotor(int speed) {
  if (speed >= 0) {
    analogWrite(R_MOTOR_IN1, speed);
    digitalWrite(R_MOTOR_IN2, LOW);
  } else {
    analogWrite(R_MOTOR_IN1, speed);
    digitalWrite(R_MOTOR_IN2, HIGH);
  }
}

// Команды движения
void moveStraight() {
  setLeftMotor(BASE_SPEED);
  setRightMotor(BASE_SPEED);
}

void sharpLeft() {
  setLeftMotor(MIN_SPEED);
  setRightMotor(BASE_SPEED + TURN_FACTOR);
}

void sharpRight() {
  setLeftMotor(BASE_SPEED + TURN_FACTOR);
  setRightMotor(MIN_SPEED);
}

void turnLeft() {
  setLeftMotor(BASE_SPEED - TURN_FACTOR);
  setRightMotor(BASE_SPEED);
}

void turnRight() {
  setLeftMotor(BASE_SPEED);
  setRightMotor(BASE_SPEED - TURN_FACTOR);
}

void correctLeft() {
  setLeftMotor(BASE_SPEED - TURN_FACTOR/2);
  setRightMotor(BASE_SPEED);
}

void correctRight() {
  setLeftMotor(BASE_SPEED);
  setRightMotor(BASE_SPEED - TURN_FACTOR/2);
}

void staticLeft() {
  // TODO: реализовать поворот на месте с использованием угла с гироскопа
  // setLeftMotor(-BASE_SPEED); // ?
  setLeftMotor(0);
  setRightMotor(BASE_SPEED);
}

void staticRight() {
  // TODO: реализовать поворот на месте с использованием угла с гироскопа
  setLeftMotor(BASE_SPEED);
  setRightMotor(0);
  // setRightMotor(-BASE_SPEED); // ?
}


void stopMotors() {
  setLeftMotor(0);
  setRightMotor(0);
}