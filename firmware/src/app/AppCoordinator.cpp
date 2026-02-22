#include "app/AppCoordinator.h"

#include "services/Logger.h"

AppCoordinator::AppCoordinator()
    : settings_(Settings::defaults()),
      buttonSim_(Pins::BUTTON_SIM_PIN, settings_),
      readySignal_(Pins::READY_SIGNAL_PIN, settings_),
      leverServo_(Pins::SERVO_PIN, settings_),
      wifiManager_(settings_),
      webUi_(),
  stateMachine_(settings_, buttonSim_, readySignal_, leverServo_)
#ifdef COFFEE_TEST_MODE
  ,
  testButtonActive_(false)
#endif
{}

void AppCoordinator::begin() {
  wifiManager_.begin();
  webUi_.onStartRequest([this]() { stateMachine_.requestStart(); });
  webUi_.onStatusRequest([this]() { return statusJson(); });
#ifdef COFFEE_TEST_MODE
  webUi_.onTestButtonPressRequest([this]() { return testButtonPress(); });
  webUi_.onTestReadyRequest([this]() { return testReady(); });
  webUi_.onTestLeverBrewRequest([this]() { return testLeverBrew(); });
  webUi_.onTestLeverRestRequest([this]() { return testLeverRest(); });
  webUi_.onTestStateResetRequest([this]() { return testStateReset(); });
#endif
  webUi_.begin();
  stateMachine_.begin();
  Logger::info("App started");
}

void AppCoordinator::loop() {
  wifiManager_.loop();
  webUi_.loop();
  stateMachine_.update();
#ifdef COFFEE_TEST_MODE
  uint32_t nowMs = millis();
  if (testButtonActive_ && testButtonTimer_.expired(nowMs)) {
    buttonSim_.setPressed(false);
    testButtonActive_ = false;
    Logger::info("Test mode: button release completed");
  }
#endif
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

#ifdef COFFEE_TEST_MODE
WebUi::JsonResponse AppCoordinator::testRejectedBusy(const char* action) const {
  String json = "{\"ok\":false,\"action\":\"";
  json += action;
  json += "\",\"error\":\"Allowed only in IDLE\"}";
  return WebUi::JsonResponse{409, json};
}

WebUi::JsonResponse AppCoordinator::testButtonPress() {
  if (!stateMachine_.isIdle()) {
    Logger::warn("Test mode: button-press rejected (not IDLE)");
    return testRejectedBusy("button-press");
  }

  buttonSim_.setPressed(true);
  testButtonTimer_.start(settings_.buttonPressMs, millis());
  testButtonActive_ = true;
  Logger::info("Test mode: simulated button press");
  return WebUi::JsonResponse{202, "{\"ok\":true,\"action\":\"button-press\"}"};
}

WebUi::JsonResponse AppCoordinator::testReady() {
  uint32_t nowMs = millis();
  bool rawActive = readySignal_.isRawActive();
  bool ready = readySignal_.isReady(nowMs);

  String json = "{\"ok\":true,\"rawActive\":";
  json += rawActive ? "true" : "false";
  json += ",\"ready\":";
  json += ready ? "true" : "false";
  json += "}";
  return WebUi::JsonResponse{200, json};
}

WebUi::JsonResponse AppCoordinator::testLeverBrew() {
  if (!stateMachine_.isIdle()) {
    Logger::warn("Test mode: lever-brew rejected (not IDLE)");
    return testRejectedBusy("lever-brew");
  }

  leverServo_.setBrewPosition();
  Logger::info("Test mode: lever moved to brew position");
  return WebUi::JsonResponse{200, "{\"ok\":true,\"action\":\"lever-brew\"}"};
}

WebUi::JsonResponse AppCoordinator::testLeverRest() {
  if (!stateMachine_.isIdle()) {
    Logger::warn("Test mode: lever-rest rejected (not IDLE)");
    return testRejectedBusy("lever-rest");
  }

  leverServo_.setRestPosition();
  Logger::info("Test mode: lever moved to rest position");
  return WebUi::JsonResponse{200, "{\"ok\":true,\"action\":\"lever-rest\"}"};
}

WebUi::JsonResponse AppCoordinator::testStateReset() {
  stateMachine_.forceIdleReset();
  if (testButtonActive_) {
    buttonSim_.setPressed(false);
    testButtonActive_ = false;
  }
  leverServo_.setRestPosition();
  Logger::info("Test mode: state reset to IDLE");
  return WebUi::JsonResponse{200, "{\"ok\":true,\"action\":\"state-reset\"}"};
}
#endif
