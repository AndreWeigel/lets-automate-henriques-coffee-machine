#include "drivers/LeverServo.h"

LeverServo::LeverServo(uint8_t pin, const Settings::RuntimeSettings& settings)
    : pin_(pin),
      restAngleDeg_(settings.servoRestAngleDeg),
      brewAngleDeg_(settings.servoBrewAngleDeg),
      minAngleDeg_(settings.servoMinAngleDeg),
      maxAngleDeg_(settings.servoMaxAngleDeg),
      pulseMinUs_(settings.servoPulseMinUs),
      pulseMaxUs_(settings.servoPulseMaxUs) {}

void LeverServo::begin() {
  servo_.setPeriodHertz(50);
  servo_.attach(pin_, pulseMinUs_, pulseMaxUs_);
  setRestPosition();
}

void LeverServo::setRestPosition() {
  setAngle(restAngleDeg_);
}

void LeverServo::setBrewPosition() {
  setAngle(brewAngleDeg_);
}

void LeverServo::setAngle(int angleDeg) {
  int clamped = constrain(angleDeg, minAngleDeg_, maxAngleDeg_);
  servo_.write(clamped);
}
