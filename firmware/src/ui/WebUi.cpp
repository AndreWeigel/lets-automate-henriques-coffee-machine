#include "ui/WebUi.h"

#include "ui/WebAssets.h"

WebUi::WebUi() : server_(80) {}

void WebUi::begin() {
  server_.on("/", HTTP_GET, [this]() { handleRoot(); });
  server_.on("/app.js", HTTP_GET, [this]() { handleAppJs(); });
  server_.on("/styles.css", HTTP_GET, [this]() { handleStyles(); });
  server_.on("/api/start", HTTP_POST, [this]() { handleStart(); });
  server_.on("/api/status", HTTP_GET, [this]() { handleStatus(); });
  server_.begin();
}

void WebUi::loop() {
  server_.handleClient();
}

void WebUi::onStartRequest(const std::function<void()>& handler) {
  startHandler_ = handler;
}

void WebUi::onStatusRequest(const std::function<String()>& handler) {
  statusHandler_ = handler;
}

void WebUi::handleRoot() {
  server_.send_P(200, "text/html", kIndexHtml);
}

void WebUi::handleAppJs() {
  server_.send_P(200, "application/javascript", kAppJs);
}

void WebUi::handleStyles() {
  server_.send_P(200, "text/css", kStylesCss);
}

void WebUi::handleStart() {
  if (startHandler_) {
    startHandler_();
  }
  server_.send(202, "application/json", "{\"ok\":true}");
}

void WebUi::handleStatus() {
  String payload = statusHandler_ ? statusHandler_() : String("{}");
  server_.send(200, "application/json", payload);
}
