#pragma once
#include "common.h"


#define MAX_SPEED   255
#define BASE_SPEED  150   // Базовая скорость
#define MIN_SPEED   100   // Минимальная скорость 
#define TURN_FACTOR 60    // Сила поворота 

void setupLineSensors();
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
void lineFollowing();


void setupLineSensors() {
  pinMode(S_LEFT_MOST, INPUT);
  pinMode(S_LEFT, INPUT);
  pinMode(S_RIGHT, INPUT);
  pinMode(S_RIGHT_MOST, INPUT);
}

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

void lineFollowing() {
  // // Чтение состояния датчиков (LOW = линия обнаружена)
  bool s1 = !(digitalRead(S_LEFT_MOST) == LOW);
  bool s2 = !(digitalRead(S_LEFT) == LOW);
  bool s3 = !(digitalRead(S_RIGHT) == LOW);
  bool s4 = !(digitalRead(S_RIGHT_MOST) == LOW);

  // Определение положения линии
  if (s1 && s2 && s3 && s4) {
    // Перекресток - продолжаем движение
    moveStraight();
  } 
  else if (s1 && s2) {
    // Линия справа - поворот направо
    turnLeft();
  } 
  else if (s3 && s4) {
    // Линия слева - поворот налево
    turnRight();
  } 
  else if (s4) {
    // Крайняя линия справа - резкий поворот направо
    sharpRight();
  } 
  else if (s1) {
    // Крайняя линия слева - резкий поворот налево
    sharpLeft();
  } 
  else if (s2 && s3) {
    // Центр линии - движение прямо
    moveStraight();
  } 
  else if (s3) {
    // Линия немного справа - небольшая коррекция направо
    correctRight();
  } 
  else if (s2) {
    // Линия немного слева - небольшая коррекция налево
    correctLeft();
  } 
  else {
    unsigned long lost_line = millis();
    while((millis()-lost_line<1000) && (s1 && s2 && s3 && s4)){
      bool s1 = !(digitalRead(S_LEFT_MOST) == LOW);
      bool s2 = !(digitalRead(S_LEFT) == LOW);
      bool s3 = !(digitalRead(S_RIGHT) == LOW);
      bool s4 = !(digitalRead(S_RIGHT_MOST) == LOW);
    }
    if (millis()-lost_line>=900){
      stopMotors();
    }
    // Линия потеряна - остановка
  } 
}

