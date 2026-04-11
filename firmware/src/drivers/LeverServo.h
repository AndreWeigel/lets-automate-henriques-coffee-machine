#pragma once

#include <Arduino.h>
#include <ESP32Servo.h>

#include "config/Settings.h"

class LeverServo {
 public:
  LeverServo(uint8_t pin, const Settings::RuntimeSettings& settings);

  void begin();
  void setRestPosition();
  void setBrewPosition();
  void setAngle(int angleDeg);

 private:
  uint8_t pin_;
  int restAngleDeg_;
  int brewAngleDeg_;
  int minAngleDeg_;
  int maxAngleDeg_;
  int pulseMinUs_;
  int pulseMaxUs_;

  Servo servo_;
};
