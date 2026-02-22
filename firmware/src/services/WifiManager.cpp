#include "services/WifiManager.h"

#include "services/Logger.h"

namespace {
const char* statusName(wl_status_t status) {
  switch (status) {
    case WL_NO_SSID_AVAIL:
      return "NO_SSID";
    case WL_CONNECTED:
      return "CONNECTED";
    case WL_CONNECT_FAILED:
      return "CONNECT_FAILED";
    case WL_CONNECTION_LOST:
      return "CONNECTION_LOST";
    case WL_DISCONNECTED:
      return "DISCONNECTED";
    case WL_IDLE_STATUS:
    default:
      return "IDLE";
  }
}

void logScanResults() {
  int count = WiFi.scanNetworks(/*async=*/false, /*hidden=*/true);
  if (count <= 0) {
    Logger::warn("WiFi scan: no networks found");
    return;
  }

  String message = String("WiFi scan: ") + count + " networks";
  Logger::info(message.c_str());
  for (int i = 0; i < count; ++i) {
    String entry = String(" - ") + WiFi.SSID(i) + " (RSSI " + WiFi.RSSI(i) + ")";
    Logger::info(entry.c_str());
  }
}
}  // namespace

WifiManager::WifiManager(const Settings::RuntimeSettings& settings)
    : ssid_(settings.wifiSsid),
      pass_(settings.wifiPass),
      apSsid_(settings.apSsid),
      apPass_(settings.apPass),
      lastStatus_(WL_IDLE_STATUS),
      lastAttemptMs_(0),
      lastLogMs_(0),
      retryIntervalMs_(15000),
      logIntervalMs_(5000) {}

void WifiManager::begin() {
  WiFi.mode(WIFI_AP_STA);

  bool apStarted = WiFi.softAP(apSsid_, apPass_);
  if (apStarted) {
    String message = String("WiFi AP ready: ") + apSsid_ + " @ " + WiFi.softAPIP().toString();
    Logger::info(message.c_str());
  } else {
    Logger::error("WiFi AP start failed");
  }

  logScanResults();
  WiFi.begin(ssid_, pass_);
  lastStatus_ = WiFi.status();
  lastAttemptMs_ = millis();
  lastLogMs_ = 0;
  Logger::info("WiFi connecting");
}

void WifiManager::loop() {
  uint32_t nowMs = millis();
  wl_status_t status = WiFi.status();
  if (status != lastStatus_) {
    if (status == WL_CONNECTED) {
      String message = String("WiFi connected: ") + WiFi.localIP().toString();
      Logger::info(message.c_str());
    } else if (lastStatus_ == WL_CONNECTED) {
      Logger::warn("WiFi disconnected");
    }
    lastStatus_ = status;
  }

  if (status != WL_CONNECTED && (nowMs - lastAttemptMs_) >= retryIntervalMs_) {
    Logger::warn("WiFi reconnecting");
    WiFi.disconnect(false, false);
    WiFi.begin(ssid_, pass_);
    lastAttemptMs_ = nowMs;
  }

  if (status != WL_CONNECTED && (nowMs - lastLogMs_) >= logIntervalMs_) {
    String message = String("WiFi status: ") + statusName(status);
    Logger::info(message.c_str());
    lastLogMs_ = nowMs;
  }
}

bool WifiManager::isConnected() const {
  return WiFi.status() == WL_CONNECTED;
}
