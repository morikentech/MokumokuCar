// MotorController.cpp
#include "MotorController.h"

MotorController::MotorController(int pwmPin, int directionPin) {
    _pwmPin = pwmPin;
    _directionPin = directionPin;
    // ここでピンの初期設定などを行う
}

void MotorController::setSpeed(int speed) {
    // モータの速度を設定するコード
}

void MotorController::stop() {
    // モータを停止するコード
}

