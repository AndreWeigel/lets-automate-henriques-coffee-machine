#include "app/AppCoordinator.h"

#include "services/Logger.h"

AppCoordinator::AppCoordinator()
    : settings_(Settings::defaults()),
      buttonSim_(Pins::BUTTON_SIM_PIN, settings_),
      readySignal_(Pins::READY_SIGNAL_PIN, settings_),
      leverServo_(Pins::SERVO_PIN, settings_),
      wifiManager_(settings_),
      webUi_(),
      stateMachine_(settings_, buttonSim_, readySignal_, leverServo_) {}

void AppCoordinator::begin() {
  wifiManager_.begin();
  webUi_.onStartRequest([this]() { stateMachine_.requestStart(); });
  webUi_.onStatusRequest([this]() { return statusJson(); });
  webUi_.begin();
  stateMachine_.begin();
  Logger::info("App started");
}

void AppCoordinator::loop() {
  wifiManager_.loop();
  webUi_.loop();
  stateMachine_.update();
}

String AppCoordinator::statusJson() {
  StateSnapshot snapshot = stateMachine_.snapshot();
  size_t stateCount = 0;
  const CoffeeState* stateOrder = stateMachine_.stateOrder(stateCount);
  String json = "{";
  json += "\"state\":\"";
  json += snapshot.name;
  json += "\"";
  json += ",\"error\":";
  json += snapshot.error ? "true" : "false";
  json += ",\"states\":[";
  for (size_t i = 0; i < stateCount; ++i) {
    if (i > 0) {
      json += ",";
    }
    json += "\"";
    json += stateMachine_.nameFor(stateOrder[i]);
    json += "\"";
  }
  json += "]";
  if (snapshot.message) {
    json += ",\"message\":\"";
    json += snapshot.message;
    json += "\"";
  }
  json += "}";
  return json;
}
