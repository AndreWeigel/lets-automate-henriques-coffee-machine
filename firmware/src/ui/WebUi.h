#pragma once

#include <Arduino.h>
#include <WebServer.h>
#include <functional>

class WebUi {
 public:
  WebUi();

  void begin();
  void loop();

  void onStartRequest(const std::function<void()>& handler);
  void onStatusRequest(const std::function<String()>& handler);

 private:
  void handleRoot();
  void handleAppJs();
  void handleStyles();
  void handleStart();
  void handleStatus();

  WebServer server_;
  std::function<void()> startHandler_;
  std::function<String()> statusHandler_;
};
