#pragma once

#include <Arduino.h>

class Logger {
 public:
  static void begin(uint32_t baudRate);
  static void info(const char* message);
  static void warn(const char* message);
  static void error(const char* message);

 private:
  static void log(const char* level, const char* message);
};
