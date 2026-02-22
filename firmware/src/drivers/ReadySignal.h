#pragma once

#include <Arduino.h>

#include "config/Settings.h"

class ReadySignal {
 public:
  ReadySignal(uint8_t pin, const Settings::RuntimeSettings& settings);

  void begin();
  void update(uint32_t nowMs);
  bool isReady(uint32_t nowMs) const;
  bool isRawActive() const;

 private:
  uint8_t pin_;
  uint32_t debounceMs_;
  uint8_t activeLevel_;
  bool usePullup_;

  bool activeObserved_;
  uint32_t activeSinceMs_;
};
