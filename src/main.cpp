#include <Arduino.h>
#include <Servo.h>
#include <Ultrasonic.h>
#include <I2Cdev.h>
#include <MPU6050_6Axis_MotionApps20.h>

#include "line_following.h"

// Пины ИК-датчиков (LOW = линия обнаружена)
#define S_LEFT_MOST  9  // IN1 -> D9
#define S_LEFT       10 // IN2 -> D10
#define S_RIGHT      11 // IN3 -> D11
#define S_RIGHT_MOST 12 // IN4 -> D12
#define SERVO_PIN 4     // D11
#define TRIG_PIN 2      // D2
#define ECHO_PIN 3      // D3

#define SERVO_DELAY_MS 300
#define SERVO_DELAY_MS_PER_DEGREE 2

Servo servo;
Ultrasonic sonar(TRIG_PIN, ECHO_PIN);
MPU6050 mpu;

uint8_t fifoBuffer[45];         // буфер
uint32_t yprTimer = 0;
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
  Serial.begin(115200);
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

bool flag = true;
uint32_t start, end;

enum mode { LINE_FOLLOWING, MAZE_SOLVING, GARBAGE_COLLECTION };
mode current_mode = MAZE_SOLVING;

void loop() {
  
    // moveStraight();
    // turnLeft();
  
    staticLeft();
    // setLeftMotor(BASE_SPEED);
    delay(500);
    stopMotors();
    delay(5000);

  // static uint32_t tmr;

  // switch (current_mode) {
  // case LINE_FOLLOWING:
  //   Serial.println("line following");
  //   // // Чтение состояния датчиков (LOW = линия обнаружена)
  //   bool s1 = (digitalRead(S_LEFT_MOST) == LOW);
  //   bool s2 = (digitalRead(S_LEFT) == LOW);
  //   bool s3 = (digitalRead(S_RIGHT) == LOW);
  //   bool s4 = (digitalRead(S_RIGHT_MOST) == LOW);

  //   // Определение положения линии
  //   if (s1 && s2 && s3 && s4) {
  //     // Перекресток - продолжаем движение
  //     moveStraight();
  //   } 
  //   else if (s1 && s2) {
  //     // Линия слева - поворот направо
  //     turnRight();
  //   } 
  //   else if (s3 && s4) {
  //     // Линия справа - поворот налево
  //     turnLeft();
  //   } 
  //   else if (s1) {
  //     // Крайняя линия слева - резкий поворот направо
  //     sharpRight();
  //   } 
  //   else if (s4) {
  //     // Крайняя линия справа - резкий поворот налево
  //     sharpLeft();
  //   } 
  //   else if (s2 && s3) {
  //     // Центр линии - движение прямо
  //     moveStraight();
  //   } 
  //   else if (s2) {
  //     // Линия немного слева - небольшая коррекция направо
  //     correctRight();
  //   } 
  //   else if (s3) {
  //     // Линия немного справа - небольшая коррекция налево
  //     correctLeft();
  //   } 
  //   else {
  //     int lost_line = millis();
  //     while((millis()-lost_line<1000) && (s1 && s2 && s3 && s4)){
  //       delay(5); 
  //     }
  //     if (millis()-lost_line>=1000){
  //       stopMotors();
  //       current_mode = MAZE_SOLVING;
  //     }
  //     // Линия потеряна - остановка
  //   } 
  //   delay(500);
  //   Serial.println('\n\n\n\n');
  //   if (s1) Serial.println("1");
  //   if (s2) Serial.println("2");
  //   if (s3) Serial.println("3");
  //   if (s4) Serial.println("4");

  //   break;
  // case MAZE_SOLVING:
  //   Serial.println("maze solving");
  //   // moveStraight();
  //   // delay(1000);
  //   // stopMotors();
  //   // delay(5000);
  //   break;
  // case GARBAGE_COLLECTION:
  //   // Serial.println("garbage collection");

    
  //   // if (millis() - tmr >= 11) {  // таймер на 11 мс (период готовности значений)
  //   //   if (mpu.dmpGetCurrentFIFOPacket(fifoBuffer)) {
  //   //     // переменные для расчёта (ypr можно вынести в глобал)
  //   //     Quaternion q;
  //   //     VectorFloat gravity;
  //   //     float ypr[3];

  //   //     // расчёты
  //   //     mpu.dmpGetQuaternion(&q, fifoBuffer);
  //   //     mpu.dmpGetGravity(&gravity, &q);
  //   //     mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);

  //   //     // выводим результат в радианах (-3.14, 3.14)
  //   //     Serial.print(ypr[0]); // вокруг оси Z
  //   //     Serial.print(',');
  //   //     Serial.print(ypr[1]); // вокруг оси Y
  //   //     Serial.print(',');
  //   //     Serial.print(ypr[2]); // вокруг оси X
  //   //     Serial.println();
  //   //     // для градусов можно использовать degrees()

  //   //     tmr = millis();   // сброс таймера
  //   //   }
  //   // }


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
  //   break;
  // }
  // delay(1000);
}