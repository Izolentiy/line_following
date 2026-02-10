#pragma once
#include "raw/concepts.h"
#include "common.h"
#include "line_following.h"

bool pulseflag = true, starterflag = true, sideflag = true;
uint32_t start, end;

void mazeSolving();

void mazeSolving() {

  servo.write(90);
  delay(SERVO_DELAY_MS);
  if(sonar.read()<25){//разворачивается на 90 градусов вправо если видит перед собой стенку
    moveBackwards();
    delay(100);
    turnRight.perform();
  }

  servo.write(180);
  delay(SERVO_DELAY_MS);
  if(sonar.read()>30){//сворачивает налево если слева нету стенки
    moveStraight();
    delay(100);//время необходимое чтобы встать в проём и не удариться об косяк(нужно регулировать в зависимости от лабиринта)
    staticLeft();
    moveStraight;
    delay(300);//время необходимое чтобы объехать стенку(нужно регулировать в зависимостиот лабиринта)
    stopMotors();
  }
  moveForwardShort.perform();
}