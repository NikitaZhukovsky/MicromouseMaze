#include <Arduino.h>
#include "digitalWriteFast.h"
/**
   Hardware pin defines
*/
#define BOARD UKMARSBOT_V1
const int ENCODER_LEFT_CLK = 2;
const int ENCODER_RIGHT_CLK = 3;
const int ENCODER_LEFT_B = 4;
const int ENCODER_RIGHT_B = 8;
const int MOTOR_LEFT_DIR = 7;
const int MOTOR_RIGHT_DIR = 8;
const int MOTOR_LEFT_PWM = 9;
const int MOTOR_RIGHT_PWM = 10;
const int LED_RIGHT = 6;
const int LED_LEFT = 11;
const int EMITTER = 12;
const int SENSOR_RIGHT_MARK = A0;
const int SENSOR_FRONT_MARK = A1;
const int SENSOR_LEFT_MARK = A5;


volatile int32_t encoderLeftCount;
volatile int32_t encoderRightCount;
int32_t encoderSum;
int32_t encoderDifference;

int FrontSensorDark = 0;
int FrontSensorLit = 0;
int FrontSensorValue = 0;

int RightSensorDark = 0;
int RightSensorLit = 0;
int RightSensorValue = 0;

int LeftSensorDark = 0;
int LeftSensorLit = 0;
int LeftSensorValue = 0;


int SPEED = 50;

double KP = 0.3;


int ReadFrontSensor() {
  FrontSensorDark = analogRead(SENSOR_FRONT_MARK);
  digitalWrite(EMITTER, 1);
  delayMicroseconds(50);
  FrontSensorLit = analogRead(SENSOR_FRONT_MARK);
  digitalWrite(EMITTER, 0);
  FrontSensorValue = FrontSensorLit - FrontSensorDark;
  return FrontSensorValue;
}

int ReadRightSensor() {
  RightSensorDark = analogRead(SENSOR_RIGHT_MARK);
  digitalWrite(EMITTER, 1);
  delayMicroseconds(50);
  RightSensorLit = analogRead(SENSOR_RIGHT_MARK);
  digitalWrite(EMITTER, 0);
  RightSensorValue = RightSensorLit - RightSensorDark;

  if ( RightSensorValue < 10) {
    RightSensorValue = 10;
  }
  if ( RightSensorValue > 180) {
    RightSensorValue = 180;
  }
  return RightSensorValue;
}

int ReadLeftSensor() {
  LeftSensorDark = analogRead(SENSOR_LEFT_MARK);
  digitalWrite(EMITTER, 1);
  delayMicroseconds(50);
  LeftSensorLit = analogRead(SENSOR_LEFT_MARK);
  digitalWrite(EMITTER, 0);
  LeftSensorValue = LeftSensorLit - LeftSensorDark;

  if ( LeftSensorValue < 10) {
    LeftSensorValue = 10;
  }
  if ( LeftSensorValue > 180) {
    LeftSensorValue = 180;
  }

  return LeftSensorValue;
}

void setLeftMotorPWM(int pwm) {
  pwm = constrain(pwm, -255, 255);
  if (pwm < 0) {
    digitalWrite(MOTOR_LEFT_DIR, 1);
    analogWrite(MOTOR_LEFT_PWM, -pwm);
  } else {
    digitalWrite(MOTOR_LEFT_DIR, 0);
    analogWrite(MOTOR_LEFT_PWM, pwm);
  }
}

void setRightMotorPWM(int pwm) {
  pwm = constrain(pwm, -255, 255);
  if (pwm < 0) {
    digitalWrite(MOTOR_RIGHT_DIR, 0);
    analogWrite(MOTOR_RIGHT_PWM, -pwm);
  } else {
    digitalWrite(MOTOR_RIGHT_DIR, 1);
    analogWrite(MOTOR_RIGHT_PWM, pwm);
  }
}


void setMotorPWM(int left, int right) {
  setLeftMotorPWM(left);
  setRightMotorPWM(right);
}
void left() {
  setMotorPWM (0, 0);
  delay(1000);
  setMotorPWM (-40, 40);
  delay(235);
  setMotorPWM (0, 0);
  delay(1000);
}
void right() {
  setMotorPWM (0, 0);
  delay(1000);
  setMotorPWM (40, -40);
  delay(220);
  setMotorPWM (0, 0);
  delay(1000);
}
void turn() {
  setMotorPWM (0, 0);
  delay(1000);
  setMotorPWM (-40, 40);
  delay(455);
  setMotorPWM (0, 0);
  delay(1000);
}
void forward() {
  setMotorPWM (0, 0);
  delay(1000);
  setMotorPWM (40, 40);
  delay(200);
  setMotorPWM (0, 0);
  delay(1000);
}

void work() {
  if (RightSensorValue > 150 && FrontSensorValue > 100 ) {
    setMotorPWM (0, 0);
    delay(1000);
    digitalWrite(LED_LEFT, HIGH);
    delay(500);
    digitalWrite(LED_LEFT, LOW);
    delay(500);
    setMotorPWM (-25, -20);
    delay(500);
    setMotorPWM (-40, 40);
    delay(500);
  }

  int delta = (RightSensorValue - 150) * KP;
  setMotorPWM (SPEED - delta, SPEED + delta);



}

void setup() {
  pinMode(MOTOR_LEFT_DIR, OUTPUT);
  pinMode(MOTOR_RIGHT_DIR, OUTPUT);
  pinMode(MOTOR_LEFT_PWM, OUTPUT);
  pinMode(MOTOR_RIGHT_PWM, OUTPUT);

  pinMode(LED_LEFT, OUTPUT);

  digitalWrite(MOTOR_LEFT_PWM, 0);
  digitalWrite(MOTOR_LEFT_DIR, 0);
  digitalWrite(MOTOR_RIGHT_PWM, 0);
  digitalWrite(MOTOR_RIGHT_DIR, 0);


  Serial.begin(9600);


  pinMode(EMITTER, OUTPUT);
  digitalWrite(EMITTER, 0); // be sure the emitter is off

  delay(2000);
}

void loop() {
  //setMotorPWM(40,40);

  ReadFrontSensor();
  ReadRightSensor();
  ReadLeftSensor();
  work();




 
}

//sensor test
/*
  Serial.print("Front: ");
  Serial.print(FrontSensorValue);
  Serial.print("  ");
  Serial.print("Right: ");
  Serial.print(RightSensorValue);
  Serial.print("  ");
  Serial.print("Left: ");
  Serial.print(6*LeftSensorValue);
  Serial.println();
  delay(1000);
*/
