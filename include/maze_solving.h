#pragma once
#include "common.h"
#include "action.h"

void mazeSolving();
void resetStates();

LinearMoveToDistance moveStraight15cm(15);
LinearMoveToDistance moveBack3cm(-3);
LinearMoveToDistance moveBack10cm(-10);
LinearMoveToDistance moveStraight10cm(10);
LinearMoveToDistance moveStraight3cm(3);

TurnInDirection turnRight45deg(PI_HALF/2.0f);
TurnInDirection turnRight180deg(PI);

/**
 * можно ли рассматривать задачу прохождения лабиринта как
 * задачу удержания расстояния от левой стены на фиксированном значении,
 * при постоянном движении вперед?
 * 
 * зададим движение вперед при удержании расстояния от правой стены
 * в определенном промежутке.
 * - смотрим текущий угол поворота.
 * 
 * 
 * поворот
 * - поворачиваемся на 30 градусов
 * - поворачиваем сонар на 30 градусов,чтобы он смотрел на стену
 * - если расстояние увелчилось, двигаемся вперед пока расстояние не то которое нужно
 * - отслеживаем смещение по X?
 * - поворачиваем еще на 30 градусов
 */
void mazeSolving() {
  resetStates();

  if (preciseDistanceOn(LEFT) > 30) {
    while (moveStraight10cm.perform() != ENDED);
    while (turnLeft.perform() != ENDED);
    while (moveStraight15cm.perform() != ENDED);
  } else if (preciseDistanceOn(FRONT) < 13) {
    while (turnRight180deg.perform() != ENDED);
    while (moveBack10cm.perform() != ENDED);
    while (turnLeft.perform() != ENDED);
  } else {
    while(moveStraight3cm.perform() != ENDED);
  }
}

void resetStates() {
  moveStraight15cm.resetState();
  moveBack3cm.resetState();
  moveStraight10cm.resetState();
  moveStraight3cm.resetState();
  turnLeft.resetState();
  turnRight.resetState();
}