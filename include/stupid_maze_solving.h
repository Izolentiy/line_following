#pragma once
#include "common.h"
#include "action.h"

LinearMoveByTimer moveStraight800ms(800);
LinearMoveByTimer moveStraight400ms(150);
LinearMoveByTimer moveBack400ms(400, false);

void makeMove(Action *action) {
    distanceOn(LEFT);
    distanceOn(FRONT);
    blockingPerform(action);
}

void repeat(Action *action, int times) {
    for (int i = 0; i < times; ++i) {
        makeMove(action);
    }
}
bool complete = false;

// 1.
void solution_1() {
    if (complete) return;

    repeat(&moveStraight400ms, 3);
    makeMove(&turnLeft);
    repeat(&moveStraight400ms, 3);
    makeMove(&turnRight);
    repeat(&moveStraight400ms, 7);
    makeMove(&moveBack400ms);
    blockingPerform(&turnLeft);
    blockingPerform(&moveStraight400ms);
    blockingPerform(&turnRight);
    repeat(&moveStraight400ms, 6);
    //каркас
    blockingPerform(&turnRight);
    blockingPerform(&moveStraight400ms);
    blockingPerform(&turnLeft);
    repeat(&moveStraight400ms,3);
    //конец далее-моментна которомнужно остановаить видео,типо продолжает работать
    blockingPerform(&turnRight);
    blockingPerform(&turnRight);

    complete = true;
}

// 2.
void solution_2(){
    repeat(&moveStraight400ms, 3);
    makeMove(&turnLeft);
    repeat(&moveStraight400ms, 3);
    makeMove(&turnRight);
    repeat(&moveStraight400ms, 7);
    makeMove(&moveBack400ms);
    blockingPerform(&turnLeft);
    blockingPerform(&moveStraight400ms);
    blockingPerform(&turnRight);
    repeat(&moveStraight400ms, 6);
    //каркас
    blockingPerform(&turnRight);
    blockingPerform(&moveStraight400ms);
    blockingPerform(&turnLeft);
    repeat(&moveStraight400ms,3);
    //дополнение к первому коду
    blockingPerform(&turnRight);
    blockingPerform(&turnRight);
    repeat(&moveStraight400ms,4);
    blockingPerform(&turnLeft);
    repeat(&moveStraight400ms,4);
    blockingPerform(&turnLeft);
    repeat(&moveStraight400ms,4);
    //остановить видео
    blockingPerform(&turnRight);
    blockingPerform(&turnRight);
}

// 3.

void solution_3(){
    repeat(&moveStraight400ms,7);
    blockingPerform(&turnRight);
    repeat(&moveStraight400ms,4);
    blockingPerform(&turnLeft);
    repeat(&moveStraight400ms,3);
    blockingPerform(&turnLeft);
    repeat(&moveStraight400ms,3);
    repeat(&moveStraight400ms,3);
    repeat(&moveStraight400ms,3);
    blockingPerform(&turnLeft);
    repeat(&moveStraight400ms,3);
    //остановить видео
    blockingPerform(&turnLeft);
    repeat(&moveStraight400ms,3);
}

void solution_4(){
    //различие началось
    repeat(&moveStraight400ms,4);
    blockingPerform(&turnLeft);
    repeat(&moveStraight400ms,4);
    blockingPerform(&turnLeft);
    repeat(&moveStraight400ms,4);
    blockingPerform(&turnRight);
    blockingPerform(&turnRight);
    //различе закончилось
    repeat(&moveStraight400ms,7);
    blockingPerform(&turnRight);
    repeat(&moveStraight400ms,4);
    blockingPerform(&turnLeft);
    repeat(&moveStraight400ms,3);
    blockingPerform(&turnLeft);
    repeat(&moveStraight400ms,3);
    repeat(&moveStraight400ms,3);
    repeat(&moveStraight400ms,3);
    blockingPerform(&turnLeft);
    repeat(&moveStraight400ms,3);
    //остановить видео
    blockingPerform(&turnLeft);
    repeat(&moveStraight400ms,3);
}
// 4.