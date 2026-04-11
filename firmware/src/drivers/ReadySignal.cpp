#include "drivers/ReadySignal.h"

ReadySignal::ReadySignal(uint8_t pin, const Settings::RuntimeSettings& settings)
    : pin_(pin),
      debounceMs_(settings.readyDebounceMs),
      activeLevel_(settings.readyActiveLevel),
      usePullup_(settings.readyUsePullup),
      activeObserved_(false),
      activeSinceMs_(0) {}

void ReadySignal::begin() {
  pinMode(pin_, usePullup_ ? INPUT_PULLUP : INPUT);
  activeObserved_ = false;
  activeSinceMs_ = 0;
}

void ReadySignal::update(uint32_t nowMs) {
  int reading = digitalRead(pin_);
  if (reading == activeLevel_) {
    if (!activeObserved_) {
      activeObserved_ = true;
      activeSinceMs_ = nowMs;
    }
  } else {
    activeObserved_ = false;
    activeSinceMs_ = 0;
  }
}

bool ReadySignal::isReady(uint32_t nowMs) const {
  if (!activeObserved_) {
    return false;
  }
  return (nowMs - activeSinceMs_) >= debounceMs_;
}

bool ReadySignal::isRawActive() const {
  return digitalRead(pin_) == activeLevel_;
}
