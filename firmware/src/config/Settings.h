#pragma once

#include <Arduino.h>

#if __has_include("secrets.h")
#include "secrets.h"
#endif

#ifndef COFFEE_WIFI_SSID
#define COFFEE_WIFI_SSID "change-me"
#endif

#ifndef COFFEE_WIFI_PASS
#define COFFEE_WIFI_PASS "change-me"
#endif

#ifndef COFFEE_AP_SSID
#define COFFEE_AP_SSID "CoffeeMachine-Setup"
#endif

#ifndef COFFEE_AP_PASS
#define COFFEE_AP_PASS "change-me-123"
#endif

#ifndef COFFEE_MDNS_HOST
#define COFFEE_MDNS_HOST "coffee"
#endif

namespace Settings {
struct RuntimeSettings {
  uint32_t buttonPressMs;
  uint32_t readyDebounceMs;
  uint32_t readyTimeoutMs;
  uint32_t brewTimeMs;
  uint32_t leverMoveMs;
  uint32_t doneHoldMs;

  int servoRestAngleDeg;
  int servoBrewAngleDeg;
  int servoMinAngleDeg;
  int servoMaxAngleDeg;
  int servoPulseMinUs;
  int servoPulseMaxUs;

  uint8_t buttonActiveLevel;
  uint8_t readyActiveLevel;
  bool readyUsePullup;

  const char* wifiSsid;
  const char* wifiPass;
  const char* apSsid;
  const char* apPass;
  const char* mdnsHost;
};

inline RuntimeSettings defaults() {
  RuntimeSettings settings{};
  settings.buttonPressMs = 300;
  settings.readyDebounceMs = 250;
  settings.readyTimeoutMs = 20000;
  settings.brewTimeMs = 25000;
  settings.leverMoveMs = 800;
  settings.doneHoldMs = 1000;

  settings.servoRestAngleDeg = 10;
  settings.servoBrewAngleDeg = 120;
  settings.servoMinAngleDeg = 0;
  settings.servoMaxAngleDeg = 180;
  settings.servoPulseMinUs = 500;
  settings.servoPulseMaxUs = 2500;

  settings.buttonActiveLevel = HIGH;
  settings.readyActiveLevel = HIGH;
  settings.readyUsePullup = true;

  settings.wifiSsid = COFFEE_WIFI_SSID;
  settings.wifiPass = COFFEE_WIFI_PASS;
  settings.apSsid = COFFEE_AP_SSID;
  settings.apPass = COFFEE_AP_PASS;
  settings.mdnsHost = COFFEE_MDNS_HOST;

  return settings;
}
}  // namespace Settings
