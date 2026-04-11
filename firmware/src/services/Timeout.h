#pragma once

#include <Arduino.h>

class Timeout {
 public:
  Timeout() : startMs_(0), durationMs_(0), running_(false) {}

  void start(uint32_t durationMs, uint32_t nowMs) {
    durationMs_ = durationMs;
    startMs_ = nowMs;
    running_ = true;
  }

  void stop() {
    running_ = false;
  }

  bool running() const {
    return running_;
  }

  bool expired(uint32_t nowMs) const {
    if (!running_) {
      return false;
    }
    return (uint32_t)(nowMs - startMs_) >= durationMs_;
  }

 private:
  uint32_t startMs_;
  uint32_t durationMs_;
  bool running_;
};
