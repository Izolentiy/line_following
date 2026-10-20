#pragma once
#include <Arduino.h>
#include <Servo.h>
#include <Ultrasonic.h>
#include <I2Cdev.h>
#include <MPU6050_6Axis_MotionApps20.h>

// ##### PINS #####
#define TRIG_PIN        2   // D2
#define ECHO_PIN        3   // D3
#define SERVO_PIN       4   // D4
#define L_MOTOR_IN1     5   // D5 (ШИМ)
#define L_MOTOR_IN2     7   // D7
#define R_MOTOR_IN1     6   // D6 (ШИМ)
#define R_MOTOR_IN2     8   // D8
#define S_RIGHT_MOST    9   // IN1 -> D9
#define S_RIGHT         10  // IN2 -> D10
#define S_LEFT          11  // IN3 -> D11
#define S_LEFT_MOST     12  // IN4 -> D12

// ##### TIMINGS #####
#define SERVO_DELAY_MS_PER_DEGREE 2
#define SONAR_DELAY_MS            100 // to prevent echo

// ##### MOTION #####
#define BASE_SPEED      150
#define PERIODIC_SPEED  150
#define TURN_SPEED      130
#define TURN_STRENGTH   1600

#define Z_GYRO_OFFSET   9
#define ANGLE_TOLERANCE 0.15f

// servo angles
#define LEFT  180
#define FRONT 90
#define RIGHT 0

#define PI_HALF 1.570796f

// ##### DEBUG OUTPUT #####
#define ROBOT_DEBUG
#ifdef ROBOT_DEBUG
char msg_buf[50];
#define PRINT(...) do { sprintf(msg_buf, __VA_ARGS__); Serial.println(msg_buf); } while (0)
// sprintf on AVR has no float support
void printValue(const char *label, float value) {
  Serial.print(label);
  Serial.println(value);
}
#else
#define PRINT(...) ((void)0)
#define printValue(label, value) ((void)0)
#endif

Servo servo;
Ultrasonic sonar(TRIG_PIN, ECHO_PIN);
MPU6050 mpu;

// gyroscope state
uint32_t angleTimer = 0;
float angles[3];
int8_t turnCount = 0; // full turns made, to get a continuous angle
uint8_t fifoBuffer[45];
float initAngle = 0.0f, currAngle = 0.0f;

// motor state
uint32_t speedTimer = 0;
int leftSpeed = 0, rightSpeed = 0;

void setupServo() {
  servo.attach(SERVO_PIN);
  servo.write(FRONT);
}

void setupGyroscope() {
  Wire.begin();
  mpu.initialize();
  mpu.dmpInitialize();
  mpu.setDMPEnabled(true);

  mpu.setZGyroOffset(Z_GYRO_OFFSET);
  Serial.println("Waiting for gyro's settling (5 seconds)");
  delay(5000);
  Serial.println("Setup is completed");
}

// ##### MOTORS #####

// for negative speeds IN2 is HIGH, so the PWM on IN1 is inverted
static void setMotor(uint8_t pwmPin, uint8_t dirPin, int speed) {
  analogWrite(pwmPin, speed);
  digitalWrite(dirPin, speed >= 0 ? LOW : HIGH);
}

void setLeftMotor(int speed) { setMotor(L_MOTOR_IN1, L_MOTOR_IN2, speed); }
void setRightMotor(int speed) { setMotor(R_MOTOR_IN1, R_MOTOR_IN2, speed); }

void stopMotors() {
  setLeftMotor(0);
  setRightMotor(0);
}

void setupMotors() {
  pinMode(L_MOTOR_IN1, OUTPUT);
  pinMode(L_MOTOR_IN2, OUTPUT);
  pinMode(R_MOTOR_IN1, OUTPUT);
  pinMode(R_MOTOR_IN2, OUTPUT);
  stopMotors();
}

void setMotorSpeeds(int left, int right) {
  leftSpeed = left;
  rightSpeed = right;
}

void applyMotorSpeeds() {
  setLeftMotor(leftSpeed);
  setRightMotor(rightSpeed);
}

// alternates between applying the speeds and stopping every `period` ms
void applyPeriodicSpeeds(uint16_t period) {
  static bool stopped = false;
  if (millis() - speedTimer >= period) {
    if (stopped) {
      stopMotors();
    } else {
      applyMotorSpeeds();
    }
    stopped = !stopped;
    speedTimer = millis();
  }
}

// ##### GYROSCOPE #####

float getAngleZ() {
  float prevAngle = angles[0];
  // данные от датчика прилетает раз в 10 мс
  if (mpu.dmpGetCurrentFIFOPacket(fifoBuffer) && millis() - angleTimer >= 11) {
    Quaternion q;
    VectorFloat gravity;

    mpu.dmpGetQuaternion(&q, fifoBuffer);
    mpu.dmpGetGravity(&gravity, &q);
    mpu.dmpGetYawPitchRoll(angles, &q, &gravity);

    angleTimer = millis();

    // yaw wraps around at +-PI, count the wraps
    float diff = angles[0] - prevAngle;
    if (abs(diff) >= PI) {
      turnCount += diff > 0.0f ? -1 : 1;
    }
  }
  return angles[0] + (2.0f * PI) * turnCount;
}

// ##### STEERING #####

void adjustSpeedsToMoveForward(int base) {
  float diffAngle = initAngle - currAngle;
  int16_t diffTerm = (int16_t)(diffAngle / PI * TURN_STRENGTH);
  if (diffAngle < 0) {
    setMotorSpeeds(base + diffTerm, base);
  } else {
    setMotorSpeeds(base, base - diffTerm);
  }
}

void adjustSpeedsToMoveBackward(int base) {
  float diffAngle = initAngle - currAngle;
  int16_t diffTerm = (int16_t)(diffAngle / PI * TURN_STRENGTH);
  if (diffAngle < 0) {
    setMotorSpeeds(-base, -base - diffTerm);
  } else {
    setMotorSpeeds(-base + diffTerm, -base);
  }
}

// ##### SONAR #####

/**
 * use only if the distance is closer than 60cm
 */
uint16_t distanceOn(int angle) {
  uint8_t initServoAngle = servo.read();
  servo.write(angle);
  delay(abs(initServoAngle - angle) * SERVO_DELAY_MS_PER_DEGREE);
  uint16_t val = sonar.read();
  PRINT("Distance at angle %d: %d", angle, val);
  return val;
}

// retries until the sonar returns a valid (non-zero) reading
uint16_t preciseDistanceOn(int angle) {
  uint8_t initServoAngle = servo.read();
  servo.write(angle);
  uint16_t val;
  do {
    delay(max(abs(initServoAngle - angle) * SERVO_DELAY_MS_PER_DEGREE, SONAR_DELAY_MS));
    val = sonar.read();
  } while (val < 1);
  PRINT("Distance at angle %d: %d (%d)", angle, val, millis());
  return val;
}
