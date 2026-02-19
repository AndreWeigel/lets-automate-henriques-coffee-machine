#include <Arduino.h>

#include "app/AppCoordinator.h"
#include "services/Logger.h"

AppCoordinator app;

void setup() {
  Logger::begin(115200);
  app.begin();
}

void loop() {
  app.loop();
}
