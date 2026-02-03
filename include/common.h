#pragma once
#include <Arduino.h>
#include <Servo.h>
#include <Ultrasonic.h>
#include <I2Cdev.h>
#include <MPU6050_6Axis_MotionApps20.h>

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

#define SERVO_DELAY_MS 200
#define SERVO_DELAY_MS_PER_DEGREE 2
#define MAX_SPEED   255

Servo servo;
Ultrasonic sonar(TRIG_PIN, ECHO_PIN);
MPU6050 mpu;

uint32_t speedTimer = 0;
uint32_t angleTimer = 0;
float angles[3];
uint8_t fifoBuffer[45];

void setupServo();
void setupGyroscope();
void setupMotors();
void stopMotors();
void setLeftMotor(int speed);
void setRightMotor(int speed);
float getAngleZ();
void setPeriodicSpeeds(int16_t left, int16_t right, uint16_t period);


void setupServo() {
  servo.attach(SERVO_PIN);
  servo.write(90); // стартовое положение
}

void setupGyroscope() {
  Wire.begin();
  mpu.initialize();
  mpu.dmpInitialize();
  mpu.setDMPEnabled(true);
}

void setupMotors() {
  pinMode(L_MOTOR_IN1, OUTPUT);
  pinMode(L_MOTOR_IN2, OUTPUT);
  pinMode(R_MOTOR_IN1, OUTPUT);
  pinMode(R_MOTOR_IN2, OUTPUT);
  
  stopMotors();
}

void setLeftMotor(int speed) {
  if (speed >= 0) {
    analogWrite(L_MOTOR_IN1, speed);
    digitalWrite(L_MOTOR_IN2, LOW);
  } else {
    analogWrite(L_MOTOR_IN1, speed);
    digitalWrite(L_MOTOR_IN2, HIGH);
  }
}

void setRightMotor(int speed) {
  if (speed >= 0) {
    analogWrite(R_MOTOR_IN1, speed);
    digitalWrite(R_MOTOR_IN2, LOW);
  } else {
    analogWrite(R_MOTOR_IN1, speed);
    digitalWrite(R_MOTOR_IN2, HIGH);
  }
}

void setLeftMotor(int speed) {
  if (speed >= 0) {
    analogWrite(L_MOTOR_IN1, speed);
    digitalWrite(L_MOTOR_IN2, LOW);
  } else {
    analogWrite(L_MOTOR_IN1, speed);
    digitalWrite(L_MOTOR_IN2, HIGH);
  }
}

void setRightMotor(int speed) {
  if (speed >= 0) {
    analogWrite(R_MOTOR_IN1, speed);
    digitalWrite(R_MOTOR_IN2, LOW);
  } else {
    analogWrite(R_MOTOR_IN1, speed);
    digitalWrite(R_MOTOR_IN2, HIGH);
  }
}

float getAngleZ() {
  // данные от датчика прилетает раз в 10 мс
  if (mpu.dmpGetCurrentFIFOPacket(fifoBuffer) && millis() - angleTimer >= 11) {
    Quaternion q;
    VectorFloat gravity;
    
    mpu.dmpGetQuaternion(&q, fifoBuffer);
    mpu.dmpGetGravity(&gravity, &q);
    mpu.dmpGetYawPitchRoll(angles, &q, &gravity);

    angleTimer = millis();
  }
  return angles[0];
}

void setPeriodicSpeeds(int16_t left, int16_t right, uint16_t period) {
  static bool flag = false;
  if (millis() - speedTimer >= period) {
    if (flag) {
      stopMotors();
    } else {
      setLeftMotor(left);
      setRightMotor(right);
    }

    flag = !flag;
    speedTimer = millis();
  }
}