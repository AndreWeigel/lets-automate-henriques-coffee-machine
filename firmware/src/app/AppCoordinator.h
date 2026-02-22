#pragma once

#include <Arduino.h>

#include "app/CoffeeStateMachine.h"
#include "config/Pins.h"
#include "config/Settings.h"
#include "drivers/ButtonSim.h"
#include "drivers/LeverServo.h"
#include "drivers/ReadySignal.h"
#include "services/WifiManager.h"
#include "services/Timeout.h"
#include "ui/WebUi.h"

class AppCoordinator {
 public:
  AppCoordinator();

  void begin();
  void loop();

 private:
  String statusJson();
  #ifdef COFFEE_TEST_MODE
    WebUi::JsonResponse testButtonPress();
    WebUi::JsonResponse testReady();
    WebUi::JsonResponse testLeverBrew();
    WebUi::JsonResponse testLeverRest();
    WebUi::JsonResponse testStateReset();
    WebUi::JsonResponse testRejectedBusy(const char* action) const;
  #endif

  Settings::RuntimeSettings settings_;
  ButtonSim buttonSim_;
  ReadySignal readySignal_;
  LeverServo leverServo_;
  WifiManager wifiManager_;
  WebUi webUi_;
  CoffeeStateMachine stateMachine_;
#ifdef COFFEE_TEST_MODE
  Timeout testButtonTimer_;
  bool testButtonActive_;
#endif
};
