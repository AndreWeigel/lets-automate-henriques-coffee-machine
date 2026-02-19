#pragma once

#include <Arduino.h>

#include "config/Settings.h"

class ButtonSim {
 public:
  ButtonSim(uint8_t pin, const Settings::RuntimeSettings& settings);

  void begin();
  void setPressed(bool pressed);
  bool isPressed() const;

 private:
  uint8_t pin_;
  uint8_t activeLevel_;
  bool pressed_;
};
