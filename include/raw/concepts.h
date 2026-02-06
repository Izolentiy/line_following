#pragma once
#include "common.h"

bool shouldTurnLeft = true;
bool rotatingRight = false;
bool rotatingLeft = false;
bool resetAngle = false;
bool missionComplete = false;

bool movingBack = true;
bool movingForward = true;
bool turningComplete = false;

float initAngle = 0.0f, currAngle = 0.0f;

enum ActionState {NOT_STARTED, IN_PROGRESS, ENDED};

class Action {
  protected:
    virtual bool shouldStop() {}
    virtual void onStart() {}
    virtual void inLoop() {}
    virtual void onEnd() {}
  public:
    ActionState state = NOT_STARTED;
    void resetState() {
      state = NOT_STARTED;
    }
    ActionState perform() {
      switch (state) {
      case NOT_STARTED:
        onStart();
        state = IN_PROGRESS;
      case IN_PROGRESS:
        if (shouldStop()) {
          onEnd();
          state = ENDED;
        } else {
          inLoop();
          state = IN_PROGRESS;
        }
      case ENDED: break;
      }
      return state;
    }
};

class ComplexAction : public Action {
  protected:
    Action *subAction = nullptr;
    int subActionCount = 0;
    int index = 0;
    bool shouldStop() override {
      return index == subActionCount;
    }
    void inLoop() override {
      if (subAction[index].perform() == ENDED) {
        subAction[index].resetState();
        ++index;
      }
    }
  public:
    ComplexAction(Action *subAction, int subActionCount) {
      this->subAction = subAction;
      this->subActionCount = subActionCount;
    }
};


// ##### ACTIONS ##### 


class TurnLeft : public Action {
  private:
    bool shouldStop() override {
      return currAngle < initAngle - PI_HALF+0.1f;
    }
    void onStart() override {
      setMotorSpeeds(-BASE_SPEED, BASE_SPEED);
      PRINT("start rotating left");
    }
    void inLoop() override {
      currAngle = getAngleZ();
      applyMotorSpeeds();
      PRINT("turning left, current angle = %f", currAngle);
      PRINT("left motor: %d", leftSpeed);
      PRINT("right motor: %d", rightSpeed);
    }
    void onEnd() override {
      initAngle = -PI_HALF;
      stopMotors();
      PRINT("end rotating left");
    }
} turnLeft;

class MoveBackwardShort : public Action {
  private:
    bool shouldStop() override {
      return ;
    }
    void onStart() override {}
    void inLoop() override {}
    void onEnd() override {}
} moveBackwardShort;

class MoveForward : public Action {
  private:
    bool shouldStop() override { return false; }
    void onStart() override {}
    void inLoop() override {}
    void onEnd() override {}
} moveForward;

Action turnAroundActions[5] = {
  moveBackwardShort,
  turnLeft,
  moveForward,
  moveBackwardShort,
  turnLeft
};
ComplexAction turnAround(turnAroundActions, 5);


/**
 * 1. every action have a stop condition.
 * 2. action may have actions which should be called every loop cycle
 * 3. action may have actions which should be called before start
 * 4. action may have actions which should be called after end
 * 
 * algorithms are an ordered list of actions
 */
void performTurningAroundToLeft() {
  // backward
  // turning left
  // forward (need precise distance to front wall)
  // backward (need precise distance to front wall)
  // turning left
  // complete
}