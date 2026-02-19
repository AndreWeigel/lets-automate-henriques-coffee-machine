#pragma once

#include <Arduino.h>

#include "app/CoffeeStateMachine.h"
#include "config/Pins.h"
#include "config/Settings.h"
#include "drivers/ButtonSim.h"
#include "drivers/LeverServo.h"
#include "drivers/ReadySignal.h"
#include "services/WifiManager.h"
#include "ui/WebUi.h"

class AppCoordinator {
 public:
  AppCoordinator();

  void begin();
  void loop();

 private:
  String statusJson();

  Settings::RuntimeSettings settings_;
  ButtonSim buttonSim_;
  ReadySignal readySignal_;
  LeverServo leverServo_;
  WifiManager wifiManager_;
  WebUi webUi_;
  CoffeeStateMachine stateMachine_;
};
