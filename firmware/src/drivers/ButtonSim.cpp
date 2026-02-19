#include "drivers/ButtonSim.h"

ButtonSim::ButtonSim(uint8_t pin, const Settings::RuntimeSettings& settings)
    : pin_(pin), activeLevel_(settings.buttonActiveLevel), pressed_(false) {}

void ButtonSim::begin() {
  pinMode(pin_, OUTPUT);
  setPressed(false);
}

void ButtonSim::setPressed(bool pressed) {
  pressed_ = pressed;
  digitalWrite(pin_, pressed_ ? activeLevel_ : !activeLevel_);
}

bool ButtonSim::isPressed() const {
  return pressed_;
}
