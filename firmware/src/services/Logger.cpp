#include "services/Logger.h"

void Logger::begin(uint32_t baudRate) {
  Serial.begin(baudRate);
  // Avoid blocking boot when ESP32 is powered without a USB host attached.
  uint32_t waitStartMs = millis();
  while (!Serial && (millis() - waitStartMs) < 1500) {
    delay(10);
  }
}

void Logger::info(const char* message) {
  log("INFO", message);
}

void Logger::warn(const char* message) {
  log("WARN", message);
}

void Logger::error(const char* message) {
  log("ERROR", message);
}

void Logger::log(const char* level, const char* message) {
  Serial.print('[');
  Serial.print(level);
  Serial.print("] ");
  Serial.println(message);
}
