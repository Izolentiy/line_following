#pragma once
#include "common.h"

#define WITH_AUTORESET 1

enum ActionState {NOT_STARTED, IN_PROGRESS, ENDING, ENDED, ABORTING, ABORTED};

class Action {
  protected:
    virtual ActionState checkState() { return IN_PROGRESS; }
    virtual void onStart() {}
    virtual void inLoop() {}
    virtual void onEnd() {}
    virtual void onAbort() {}
  public:
    ActionState state = NOT_STARTED;
    void resetState() {
      state = NOT_STARTED;
    }
    ActionState perform(bool autoReset = 0) {
      switch (state) {
      case NOT_STARTED:
        onStart();
        state = IN_PROGRESS;
        break;
      case IN_PROGRESS:
        inLoop();
        state = checkState();
        break;
      case ENDING:
        onEnd();
        if (autoReset) {
          state = NOT_STARTED;
          return ENDED;
        }
        state = ENDED;
        break;
      case ABORTING: 
        onAbort();
        if (autoReset) {
          state = NOT_STARTED;
          return ABORTED;
        }
        state = ABORTED;
        break;
      case ENDED: break;
      case ABORTED: break;
      }
      return state;
    }
};

class ComplexAction : public Action {
  protected:
    Action **steps = nullptr;
    int stepCount = 0;
    int index = 0;
    bool isCyclic = false;
    bool abortOccured = false;
    ActionState checkState() override {
      if (abortOccured) return ENDING;
      if (index == stepCount) {
        if (isCyclic) index = 0;
        else return ENDING;
      }
      return IN_PROGRESS;
    }
    void inLoop() override {
      ActionState result = steps[index]->perform();
      if (result == ENDED) {
        steps[index]->resetState();
        ++index;
      } else if (result == ABORTED) {
        abortOccured = true;
      }
    }
  public:
    ComplexAction(Action **steps, int stepCount, bool isCyclic = false) {
      this->steps = steps;
      this->stepCount = stepCount;
      this->isCyclic = isCyclic;
    }
};

void blockingPerform(Action *action) {
  ActionState result;
  do {
    result = action->perform(WITH_AUTORESET);
    PRINT("%d",int(result));
  } while (result != ENDED && result != ABORTED);
}

// ##### ACTIONS ##### 

class TurnInDirection : public Action {
  private:
    float direction = 0.0f;
    ActionState checkState() override {
      if (abs(initAngle+direction - currAngle) < ANGLE_TOLERANCE) {
        stopMotors();
        delay(50);
        currAngle = getAngleZ();
        if (abs(initAngle+direction - currAngle) > ANGLE_TOLERANCE) return IN_PROGRESS; 
        PRINT("ENDING of rotation: %d", millis());
        return ENDING;
      }
      return IN_PROGRESS;
    }
    void onStart() override {
      PRINT("\nSTART of rotation\n");
    }
    void inLoop() override {
      currAngle = getAngleZ();
      if (initAngle+direction - currAngle > 0) {
        setMotorSpeeds(TURN_SPEED, -TURN_SPEED);
      } else {
        setMotorSpeeds(-TURN_SPEED, TURN_SPEED);
      }
      applyPeriodicSpeeds(75);
      Serial.print("IN_LOOP current angle: "); Serial.println(currAngle);
      Serial.print("IN_LOOP desired angle: "); Serial.println(initAngle+direction);
      PRINT("IN_LOOP apply left speed: %d", leftSpeed);
      PRINT("IN_LOOP apply right speed: %d", rightSpeed);
    }
    void onEnd() override {
      initAngle += direction;
      stopMotors();
      PRINT("END of rotation: %d", millis());
    }
  public:
    TurnInDirection(float direction) {
      this->direction = direction;
    }
};

class LinearMoveToDistance : public Action {
  private:
    int distance = 0;
    int distanceAtStart = 0;
    int currDistance = 0;
    ActionState checkState() override {
      if (abs(distanceAtStart-distance - currDistance) <= 2) return ENDING;
      if (distanceAtStart-distance <= 4 && currDistance < 6) return ABORTING;
      return IN_PROGRESS;
    }
    void onStart() override {
      distanceAtStart = distanceOn(FRONT);
      PRINT("\nSTART of linear movement\n");
    }
    void inLoop() override {
      currDistance = distanceOn(FRONT);
      currAngle = getAngleZ();
      int distanceDiff = distanceAtStart-distance - currDistance;
      int speed = PERIODIC_SPEED;
      // if (abs(distanceDiff) < 10) speed = MIN_SPEED;
      if (distanceDiff > 0) {
        adjustSpeedsToMoveBackward(speed);
      } else {
        adjustSpeedsToMoveForward(speed);
      }
      applyPeriodicSpeeds(75);
      Serial.print("IN_LOOP current angle: "); Serial.println(currAngle);
      PRINT("IN_LOOP desired distance: %d", distanceAtStart-distance);
      PRINT("IN_LOOP current distance: %d", currDistance);
      PRINT("IN_LOOP apply left speed: %d", leftSpeed);
      PRINT("IN_LOOP apply right speed: %d", rightSpeed);
    }
    void onEnd() override {
      stopMotors();
      PRINT("END of linear movement");
    }
    void onAbort() override { 
      stopMotors(); 
      PRINT("ABORT of linear movement");
    }
  public:
    LinearMoveToDistance(int distance) {
      this->distance = distance;
    }
};

class CheckSpaceInDirection : public Action {
  private:
    int direction = 0;
    ActionState checkState() override {
      if (distanceOn(direction) < 25) return ABORTING;
      return ENDED;
    }
  public:
    CheckSpaceInDirection(int direction) {
      this->direction = direction;
    }
};

class MoveForwardTillWall : public Action {
  private:
    ActionState checkState() override {
      if (distanceOn(FRONT) < 25) return ENDING;
      return IN_PROGRESS;
    }
    void inLoop() override {
      currAngle = getAngleZ();
      adjustSpeedsToMoveForward(BASE_SPEED);
      applyMotorSpeeds();
      Serial.print("moving forward currAngle: "); Serial.println(currAngle);
      Serial.print("moving forward initAngle: "); Serial.println(initAngle);
      PRINT("left motor: %d", leftSpeed);
      PRINT("right motor: %d", rightSpeed);
    }
    void onEnd() override {
      stopMotors();
      PRINT("END wall ahead found");
    }
} moveForwardTillWall;

TurnInDirection turnLeft(-PI_HALF);
TurnInDirection turnRight(PI_HALF);