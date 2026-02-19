#pragma once

#include <Arduino.h>
#include <WiFi.h>

#include "config/Settings.h"

class WifiManager {
 public:
  explicit WifiManager(const Settings::RuntimeSettings& settings);

  void begin();
  void loop();
  bool isConnected() const;

 private:
  const char* ssid_;
  const char* pass_;
  wl_status_t lastStatus_;
    uint32_t lastAttemptMs_;
    uint32_t lastLogMs_;
    uint32_t retryIntervalMs_;
    uint32_t logIntervalMs_;
};
