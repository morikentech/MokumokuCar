#include <M5Core2.h>
#include <PS4Controller.h>
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *motor1 = AFMS.getMotor(1);
Adafruit_DCMotor *motor2 = AFMS.getMotor(2);
Adafruit_DCMotor *motor3 = AFMS.getMotor(3);
Adafruit_DCMotor *motor4 = AFMS.getMotor(4);

int deadZone = 20;

void setAllMotors(int speed, uint8_t direction) {
  Adafruit_DCMotor *motors[] = {motor1, motor2, motor3, motor4};
  for (Adafruit_DCMotor *motor : motors) {
    motor->setSpeed(speed);
    motor->run(direction);
  }
}

void setMotorsForSidewaysMovement(int speed, bool isRight) {
  if (isRight) {
    motor1->setSpeed(speed);
    motor1->run(FORWARD);
    motor2->setSpeed(speed);
    motor2->run(BACKWARD);
    motor3->setSpeed(speed);
    motor3->run(FORWARD);
    motor4->setSpeed(speed);
    motor4->run(BACKWARD);
  } else {
    motor1->setSpeed(speed);
    motor1->run(BACKWARD);
    motor2->setSpeed(speed);
    motor2->run(FORWARD);
    motor3->setSpeed(speed);
    motor3->run(BACKWARD);
    motor4->setSpeed(speed);
    motor4->run(FORWARD);
  }
}

void setMotorsForRotation(int speed, bool isClockwise) {
  if (isClockwise) {
    motor1->setSpeed(speed);
    motor1->run(BACKWARD);
    motor2->setSpeed(speed);
    motor2->run(BACKWARD);
    motor3->setSpeed(speed);
    motor3->run(FORWARD);
    motor4->setSpeed(speed);
    motor4->run(FORWARD);
  } else {
    motor1->setSpeed(speed);
    motor1->run(FORWARD);
    motor2->setSpeed(speed);
    motor2->run(FORWARD);
    motor3->setSpeed(speed);
    motor3->run(BACKWARD);
    motor4->setSpeed(speed);
    motor4->run(BACKWARD);
  }
}


void setup() {
  M5.begin();
  M5.Lcd.setTextSize(2);
  M5.Lcd.setTextColor(WHITE);
  Serial.begin(9600);
  Serial.println("Adafruit Motorshield v2 - DC Motor test!");

  if (!AFMS.begin()) {
    Serial.println("Could not find Motor Shield. Check wiring.");
    while (1);
  }
  Serial.println("Motor Shield found.");

  M5.Lcd.println("Initializing PS4 Controller...");
  if (!PS4.begin("3C:61:05:0D:C6:1A")) {
    M5.Lcd.println("Failed to initialize controller");
  } else {
    M5.Lcd.println("Controller Initialized");
    Serial.println("Controller Initialized");
  }
}

void loop() {
  M5.update();

  if (PS4.isConnected()) {
    int leftStickX = PS4.data.analog.stick.lx;
    int leftStickY = PS4.data.analog.stick.ly;
    int rightStickX = PS4.data.analog.stick.rx;

    // デッドゾーン適用
    if (abs(leftStickX) < deadZone) leftStickX = 0;
    if (abs(leftStickY) < deadZone) leftStickY = 0;
    if (abs(rightStickX) < deadZone) rightStickX = 0;

    int motorSpeedX = map(abs(leftStickX), 0, 127, 0, 127);
    int motorSpeedY = map(abs(leftStickY), 0, 127, 0, 100);
    int rotationSpeed = map(abs(rightStickX), 0, 127, 0, 127);

        // モーター速度のシリアル出力
    Serial.print("Motor Speed X: ");
    Serial.println(motorSpeedX);
    Serial.print("Motor Speed Y: ");
    Serial.println(motorSpeedY);
    Serial.print("Rotation Speed: ");
    Serial.println(rotationSpeed);

    int absLeftStickX = abs(leftStickX);
    int absLeftStickY = abs(leftStickY);

    if (absLeftStickX > deadZone || absLeftStickY > deadZone) {
      if (absLeftStickX > absLeftStickY) {
        setMotorsForSidewaysMovement(motorSpeedX, leftStickX > 0); 
      } else {
        setAllMotors(motorSpeedY, leftStickY > 0 ? FORWARD : BACKWARD);
      }
    } else {
      setAllMotors(0, RELEASE);
    }
    
    if (abs(rightStickX) > deadZone) {
      setMotorsForRotation(rotationSpeed, rightStickX > 0);
      }

  } else {
    Serial.println("PS4 Controller Disconnected");
    delay(1000);
  }
}
