#include <Arduino.h>
#include <Servo.h>
#include <Ultrasonic.h>
#include <I2Cdev.h>
#include <MPU6050_6Axis_MotionApps20.h>

#include "line_following.h"

// Пины ИК-датчиков (LOW = линия обнаружена)
#define S_LEFT_MOST  12 // IN4 -> D12
#define S_LEFT       11 // IN3 -> D11
#define S_RIGHT      10 // IN2 -> D10
#define S_RIGHT_MOST  9 // IN1 -> D9
#define SERVO_PIN 4     // D11
#define TRIG_PIN 2      // D2
#define ECHO_PIN 3      // D3

#define SERVO_DELAY_MS 200
#define SERVO_DELAY_MS_PER_DEGREE 2

Servo servo;
Ultrasonic sonar(TRIG_PIN, ECHO_PIN);
MPU6050 mpu;

uint8_t fifoBuffer[45];         // буфер
uint32_t yprTimer = 0,tmr=0;
float ypr[3];

float getAngleZ() {
  // данные от датчика прилетает раз в 10 мс
  if (mpu.dmpGetCurrentFIFOPacket(fifoBuffer) && millis() - yprTimer >= 11) {
    Quaternion q;
    VectorFloat gravity;
    
    mpu.dmpGetQuaternion(&q, fifoBuffer);
    mpu.dmpGetGravity(&gravity, &q);
    mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);

    yprTimer = millis();
  }
  return ypr[0];
}

void setup() {
  Serial.begin(9600);
  Wire.begin();
  
  // инициализация DMP
  mpu.initialize();
  mpu.dmpInitialize();
  mpu.setDMPEnabled(true);

  // Инициализация датчиков
  pinMode(S_LEFT_MOST, INPUT);
  pinMode(S_LEFT, INPUT);
  pinMode(S_RIGHT, INPUT);
  pinMode(S_RIGHT_MOST, INPUT);
  
  // Инициализация моторов
  setupMotors();
  
  servo.attach(SERVO_PIN);
  // servo.write(LOOK_FORWARD);// Стартовое положение
  
  // servo.write(0);
  // delay(1000);

  // Начальная остановка
  stopMotors();
}

bool pulseflag = true,starterflag=true,sideflag=true;
uint32_t start, end;

enum mode { LINE_FOLLOWING, MAZE_SOLVING, GARBAGE_COLLECTION };
mode current_mode = MAZE_SOLVING;

void loop() {
  switch (current_mode) {
    case LINE_FOLLOWING:{
      // // Чтение состояния датчиков (LOW = линия обнаружена)
      bool s1 = !(digitalRead(S_LEFT_MOST) == LOW);
      bool s2 = !(digitalRead(S_LEFT) == LOW);
      bool s3 = !(digitalRead(S_RIGHT) == LOW);
      bool s4 = !(digitalRead(S_RIGHT_MOST) == LOW);

      // Определение положения линии
      if (s1 && s2 && s3 && s4) {
        // Перекресток - продолжаем движение
        moveStraight();
      } 
      else if (s1 && s2) {
        // Линия справа - поворот направо
        turnLeft();
      } 
      else if (s3 && s4) {
        // Линия слева - поворот налево
        turnRight();
      } 
      else if (s4) {
        // Крайняя линия справа - резкий поворот направо
        sharpRight();
      } 
      else if (s1) {
        // Крайняя линия слева - резкий поворот налево
        sharpLeft();
      } 
      else if (s2 && s3) {
        // Центр линии - движение прямо
        moveStraight();
      } 
      else if (s3) {
        // Линия немного справа - небольшая коррекция направо
        correctRight();
      } 
      else if (s2) {
        // Линия немного слева - небольшая коррекция налево
        correctLeft();
      } 
      else {
        unsigned long lost_line = millis();
        while((millis()-lost_line<1000) && (s1 && s2 && s3 && s4)){
          bool s1 = !(digitalRead(S_LEFT_MOST) == LOW);
          bool s2 = !(digitalRead(S_LEFT) == LOW);
          bool s3 = !(digitalRead(S_RIGHT) == LOW);
          bool s4 = !(digitalRead(S_RIGHT_MOST) == LOW);
        }
        if (millis()-lost_line>=900){
          stopMotors();
        }
        // Линия потеряна - остановка
      } 
      break;
    }

    case MAZE_SOLVING:{

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

      break;
    }

    case GARBAGE_COLLECTION:{
      if (starterflag){
        staticRight();
        servo.write(90);
        while(sonar.read()>30){moveStraight();}
        staticLeft();
      }
      servo.write(90);
      if(sonar.read()>30){
        moveStraight();
      }
      else{
        servo.write(90-90*(-1)^(sideflag));
        delay(SERVO_DELAY_MS);
        if(sonar.read()<30){//Отправка в стартовую точку
        }
        else if(sideflag){
          staticLeft();
          moveStraight();
          delay(1000);//время чтобы переехвать в другой ряд
          staticLeft();
          sideflag!=sideflag;
        }
        else{
          staticRight();
          moveStraight();
          delay(1000);//время чтобы переехвать в другой ряд
          staticRight();
          sideflag!=sideflag;
        }
      }
    }

    
  

  //   // servo.write(0);
  //   // delay(SERVO_DELAY_MS);
  //   // Serial.println(sonar.read());
  //   // servo.write(180);
  //   // delay(SERVO_DELAY_MS);
  //   // Serial.println(sonar.read());


  //   // servo.write(180);
  //   // delay(SERVO_DELAY_MS);
  //   // Serial.println(sonar.read());
  //   // servo.write(90);
  //   // delay(SERVO_DELAY_MS);
  //   // Serial.println(sonar.read());
  //   // servo.write(0);
  //   // delay(SERVO_DELAY_MS);
  //   // Serial.println(sonar.read());
  }
}
