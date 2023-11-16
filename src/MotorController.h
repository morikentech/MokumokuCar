// MotorController.h
#ifndef MOTORCONTROLLER_H
#define MOTORCONTROLLER_H

class MotorController {
public:
    MotorController(int pwmPin, int directionPin);
    void setSpeed(int speed);
    void stop();
private:
    int _pwmPin;
    int _directionPin;
};

#endif
