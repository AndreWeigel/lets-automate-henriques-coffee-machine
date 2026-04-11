#include "ui/WebUi.h"

#include "ui/WebAssets.h"

WebUi::WebUi() : server_(80) {}

void WebUi::begin() {
  server_.on("/", HTTP_GET, [this]() { handleRoot(); });
  server_.on("/app.js", HTTP_GET, [this]() { handleAppJs(); });
  server_.on("/styles.css", HTTP_GET, [this]() { handleStyles(); });
  server_.on("/api/start", HTTP_POST, [this]() { handleStart(); });
  server_.on("/api/status", HTTP_GET, [this]() { handleStatus(); });
#ifdef COFFEE_TEST_MODE
  server_.on("/api/test/button-press", HTTP_POST, [this]() { handleTestButtonPress(); });
  server_.on("/api/test/ready", HTTP_GET, [this]() { handleTestReady(); });
  server_.on("/api/test/lever-brew", HTTP_POST, [this]() { handleTestLeverBrew(); });
  server_.on("/api/test/lever-rest", HTTP_POST, [this]() { handleTestLeverRest(); });
  server_.on("/api/test/state-reset", HTTP_POST, [this]() { handleTestStateReset(); });
#endif
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

#ifdef COFFEE_TEST_MODE
void WebUi::onTestButtonPressRequest(const std::function<JsonResponse()>& handler) {
  testButtonPressHandler_ = handler;
}

void WebUi::onTestReadyRequest(const std::function<JsonResponse()>& handler) {
  testReadyHandler_ = handler;
}

void WebUi::onTestLeverBrewRequest(const std::function<JsonResponse()>& handler) {
  testLeverBrewHandler_ = handler;
}

void WebUi::onTestLeverRestRequest(const std::function<JsonResponse()>& handler) {
  testLeverRestHandler_ = handler;
}

void WebUi::onTestStateResetRequest(const std::function<JsonResponse()>& handler) {
  testStateResetHandler_ = handler;
}
#endif

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

#ifdef COFFEE_TEST_MODE
void WebUi::handleTestButtonPress() {
  JsonResponse response = testButtonPressHandler_ ? testButtonPressHandler_()
                                                  : JsonResponse{501, "{\"ok\":false,\"error\":\"Not configured\"}"};
  server_.send(response.statusCode, "application/json", response.payload);
}

void WebUi::handleTestReady() {
  JsonResponse response = testReadyHandler_ ? testReadyHandler_()
                                            : JsonResponse{501, "{\"ok\":false,\"error\":\"Not configured\"}"};
  server_.send(response.statusCode, "application/json", response.payload);
}

void WebUi::handleTestLeverBrew() {
  JsonResponse response = testLeverBrewHandler_ ? testLeverBrewHandler_()
                                                : JsonResponse{501, "{\"ok\":false,\"error\":\"Not configured\"}"};
  server_.send(response.statusCode, "application/json", response.payload);
}

void WebUi::handleTestLeverRest() {
  JsonResponse response = testLeverRestHandler_ ? testLeverRestHandler_()
                                                : JsonResponse{501, "{\"ok\":false,\"error\":\"Not configured\"}"};
  server_.send(response.statusCode, "application/json", response.payload);
}

void WebUi::handleTestStateReset() {
  JsonResponse response = testStateResetHandler_ ? testStateResetHandler_()
                                                 : JsonResponse{501, "{\"ok\":false,\"error\":\"Not configured\"}"};
  server_.send(response.statusCode, "application/json", response.payload);
}
#endif
