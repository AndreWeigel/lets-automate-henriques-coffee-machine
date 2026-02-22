#pragma once

#include <Arduino.h>
#include <WebServer.h>
#include <functional>

class WebUi {
 public:
  struct JsonResponse {
    int statusCode;
    String payload;
  };

  WebUi();

  void begin();
  void loop();

  void onStartRequest(const std::function<void()>& handler);
  void onStatusRequest(const std::function<String()>& handler);
#ifdef COFFEE_TEST_MODE
  void onTestButtonPressRequest(const std::function<JsonResponse()>& handler);
  void onTestReadyRequest(const std::function<JsonResponse()>& handler);
  void onTestLeverBrewRequest(const std::function<JsonResponse()>& handler);
  void onTestLeverRestRequest(const std::function<JsonResponse()>& handler);
  void onTestStateResetRequest(const std::function<JsonResponse()>& handler);
#endif

 private:
  void handleRoot();
  void handleAppJs();
  void handleStyles();
  void handleStart();
  void handleStatus();
#ifdef COFFEE_TEST_MODE
  void handleTestButtonPress();
  void handleTestReady();
  void handleTestLeverBrew();
  void handleTestLeverRest();
  void handleTestStateReset();
#endif

  WebServer server_;
  std::function<void()> startHandler_;
  std::function<String()> statusHandler_;
#ifdef COFFEE_TEST_MODE
  std::function<JsonResponse()> testButtonPressHandler_;
  std::function<JsonResponse()> testReadyHandler_;
  std::function<JsonResponse()> testLeverBrewHandler_;
  std::function<JsonResponse()> testLeverRestHandler_;
  std::function<JsonResponse()> testStateResetHandler_;
#endif
};
