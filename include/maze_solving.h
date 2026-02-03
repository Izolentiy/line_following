#pragma once
#include "common.h"
#include "line_following.h"


bool pulseflag = true, starterflag = true, sideflag = true;
uint32_t start, end;

void mazeSolving();

void mazeSolving() {
  moveStraight();

  servo.write(90);
  delay(SERVO_DELAY_MS);
  if(sonar.read()<20){//разворачивается на 180 градусов если видит перед собой стенку
    staticRight();
    pulseflag=true;
  }

  servo.write(180);
  delay(SERVO_DELAY_MS);
  if(sonar.read()>30){//сворачивает налево если слева нету стенки
    //время необходимое чтобы встать в проём и не удариться об косяк(нужно регулировать в зависимости от лабиринта)
    staticLeft();
    pulseflag=true;
    delay(300);//время необходимое чтобы объехать стенку(нужно регулировать в зависимостиот лабиринта)
  }
}