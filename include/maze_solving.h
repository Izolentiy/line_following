#pragma once
#include "common.h"
#include "action.h"

void mazeSolving();
void mazeSolvingLeft();
void mazeSolvingRight();

LinearMoveToDistance moveStraight15cm(15);
LinearMoveToDistance moveBack3cm(-3);
LinearMoveToDistance moveBack10cm(-10);
LinearMoveToDistance moveStraight8cm(8);
LinearMoveToDistance moveStraight3cm(3);

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
  mazeSolvingLeft();
}

void mazeSolvingLeft() {
  if (preciseDistanceOn(LEFT) > 30) {
    PRINT("\nTURNING LEFT\n");
    blockingPerform(&moveStraight8cm);
    blockingPerform(&turnLeft);
    blockingPerform(&moveStraight15cm);
  } else if (preciseDistanceOn(FRONT) < 10) {
    PRINT("\nTURNING RIGHT\n");
    blockingPerform(&turnRight);
  } else {
    PRINT("\nMOVING FORWARD\n");
    blockingPerform(&moveStraight3cm);
  }
}

void mazeSolvingRight() {
  if (preciseDistanceOn(RIGHT) > 30) {
    PRINT("\nTURNING RIGHT\n");
    blockingPerform(&moveStraight8cm);
    blockingPerform(&turnRight);
    blockingPerform(&moveStraight15cm);
  } else if (preciseDistanceOn(FRONT) < 10) {
    PRINT("\nTURNING RIGHT\n");
    blockingPerform(&turnLeft);
  } else {
    PRINT("\nMOVING FORWARD\n");
    blockingPerform(&moveStraight3cm);
  }
}