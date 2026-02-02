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
void slightStraight();
void sharpLeft();
void sharpRight();
void turnLeft();
void turnRight();
void correctLeft();
void correctRight();
void staticLeft();
void staticRight();

float getAngleZ();



float startAngle = 0.0f;


#define BASE_SPEED 120  // Базовая скорость
#define MIN_SPEED 100   // Минимальная скорость 
#define TURN_FACTOR 60  // Сила поворота 

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

void slightStraight(){
  setLeftMotor(MIN_SPEED);
  setRightMotor(MIN_SPEED);
}

void sharpLeft() {
  setLeftMotor(-BASE_SPEED);
  setRightMotor(BASE_SPEED);
}

void sharpRight() {
  setLeftMotor(BASE_SPEED);
  setRightMotor(-BASE_SPEED);
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

void staticLeft() {//накапливает разницу в изначальном и нынешнем угле робота чтобы получить 90 градусный поворот
  float AngleDiffer=0,prevAngle=getAngleZ(),nowAngle;
  setLeftMotor(-BASE_SPEED);
  setRightMotor(BASE_SPEED);
  while (AngleDiffer<3.14/2){
    nowAngle=getAngleZ();
    AngleDiffer+=abs(abs(prevAngle)-abs(nowAngle));
    prevAngle=nowAngle;
  }
  stopMotors();
}

void staticRight() {//работает по такому же приницпу как и staticRight но в правую сторону сторону
  float AngleDiffer=0,prevAngle=getAngleZ(),nowAngle;
  setLeftMotor(BASE_SPEED);
  setRightMotor(-BASE_SPEED);
  while (AngleDiffer<3.14/2){
    nowAngle=getAngleZ();
    AngleDiffer+=abs(abs(prevAngle)-abs(nowAngle));
    prevAngle=nowAngle;
  }
  stopMotors();
}


void stopMotors() {
  setLeftMotor(0);
  setRightMotor(0);
}

