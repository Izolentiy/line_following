#pragma once
#include "common.h"

#define WITH_AUTORESET true

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
    ActionState perform(bool autoReset = false) {
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
      case ENDED:
      case ABORTED:
        break;
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
    ComplexAction(Action **steps, int stepCount, bool isCyclic = false)
      : steps(steps), stepCount(stepCount), isCyclic(isCyclic) {}
};

ActionState blockingPerform(Action *action) {
  ActionState result;
  do {
    result = action->perform(WITH_AUTORESET);
  } while (result != ENDED && result != ABORTED);
  return result;
}

// ##### ACTIONS #####

class TurnInDirection : public Action {
  private:
    float direction = 0.0f;
    ActionState checkState() override {
      if (abs(initAngle + direction - currAngle) < ANGLE_TOLERANCE) {
        stopMotors();
        delay(50);
        currAngle = getAngleZ();
        if (abs(initAngle + direction - currAngle) > ANGLE_TOLERANCE) return IN_PROGRESS;
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
      if (initAngle + direction - currAngle > 0) {
        setMotorSpeeds(TURN_SPEED, -TURN_SPEED);
      } else {
        setMotorSpeeds(-TURN_SPEED, TURN_SPEED);
      }
      applyPeriodicSpeeds(75);
      printValue("IN_LOOP current angle: ", currAngle);
      printValue("IN_LOOP desired angle: ", initAngle + direction);
      PRINT("IN_LOOP speeds: %d %d", leftSpeed, rightSpeed);
    }
    void onEnd() override {
      initAngle += direction;
      stopMotors();
      PRINT("END of rotation: %d", millis());
    }
  public:
    TurnInDirection(float direction) : direction(direction) {}
};

class LinearMoveToDistance : public Action {
  private:
    int distance = 0;
    int distanceAtStart = 0;
    int currDistance = 0;
    ActionState checkState() override {
      if (abs(distanceAtStart - distance - currDistance) <= 2) return ENDING;
      if (distanceAtStart - distance <= 4 && currDistance < 6) return ABORTING;
      return IN_PROGRESS;
    }
    void onStart() override {
      distanceAtStart = distanceOn(FRONT);
      PRINT("\nSTART of linear movement\n");
    }
    void inLoop() override {
      stopMotors();
      currDistance = preciseDistanceOn(FRONT);
      currAngle = getAngleZ();
      int distanceDiff = distanceAtStart - distance - currDistance;
      if (distanceDiff > 0) {
        adjustSpeedsToMoveBackward(PERIODIC_SPEED);
      } else {
        adjustSpeedsToMoveForward(PERIODIC_SPEED);
      }
      applyPeriodicSpeeds(75);
      printValue("IN_LOOP current angle: ", currAngle);
      PRINT("IN_LOOP desired distance: %d", distanceAtStart - distance);
      PRINT("IN_LOOP current distance: %d", currDistance);
      PRINT("IN_LOOP speeds: %d %d", leftSpeed, rightSpeed);
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
    LinearMoveToDistance(int distance) : distance(distance) {}
};

class LinearMoveByTimer : public Action {
  private:
    unsigned long startTime = 0;
    unsigned long moveTime = 0;
    bool straight = true;
    ActionState checkState() override {
      if (millis() - startTime > moveTime) return ENDING;
      return IN_PROGRESS;
    }
    void onStart() override {
      startTime = millis();
    }
    void inLoop() override {
      currAngle = getAngleZ();
      if (straight) adjustSpeedsToMoveForward(BASE_SPEED);
      else adjustSpeedsToMoveBackward(BASE_SPEED);
      applyMotorSpeeds();
      printValue("IN_LOOP current angle: ", currAngle);
      PRINT("IN_LOOP speeds: %d %d", leftSpeed, rightSpeed);
    }
    void onEnd() override {
      stopMotors();
      PRINT("END of linear movement");
    }
  public:
    LinearMoveByTimer(unsigned long moveTime, bool straight = true)
      : moveTime(moveTime), straight(straight) {}
};

class CheckSpaceInDirection : public Action {
  private:
    int direction = 0;
    ActionState checkState() override {
      if (distanceOn(direction) < 25) return ABORTING;
      return ENDED;
    }
  public:
    CheckSpaceInDirection(int direction) : direction(direction) {}
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
      printValue("moving forward currAngle: ", currAngle);
      printValue("moving forward initAngle: ", initAngle);
      PRINT("speeds: %d %d", leftSpeed, rightSpeed);
    }
    void onEnd() override {
      stopMotors();
      PRINT("END wall ahead found");
    }
} moveForwardTillWall;

TurnInDirection turnLeft(-PI_HALF);
TurnInDirection turnRight(PI_HALF);

// short timed moves shared by the maze and garbage collection modes
LinearMoveByTimer moveStraight3cm(150);
LinearMoveByTimer moveStraight5cm(250);
