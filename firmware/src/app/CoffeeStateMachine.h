#pragma once

#include <Arduino.h>

#include "config/Settings.h"
#include "drivers/ButtonSim.h"
#include "drivers/LeverServo.h"
#include "drivers/ReadySignal.h"
#include "services/Timeout.h"

enum class CoffeeState : uint8_t {
  IDLE,
  PRESS_BUTTON,
  WAIT_READY,
  ACTUATE_LEVER,
  BREWING,
  RETURN_LEVER,
  DONE,
  ERROR
};

struct StateSnapshot {
  CoffeeState state;
  const char* name;
  bool error;
  const char* message;
};

class CoffeeStateMachine {
 public:
  CoffeeStateMachine(const Settings::RuntimeSettings& settings,
                     ButtonSim& buttonSim,
                     ReadySignal& readySignal,
                     LeverServo& leverServo);

  void begin();
  void update();
  void requestStart();

  StateSnapshot snapshot() const;
  const CoffeeState* stateOrder(size_t& count) const;
  const char* nameFor(CoffeeState state) const;

 private:
  void transitionTo(CoffeeState next, uint32_t nowMs);
  const char* stateName(CoffeeState state) const;

  const Settings::RuntimeSettings& settings_;
  ButtonSim& buttonSim_;
  ReadySignal& readySignal_;
  LeverServo& leverServo_;

  CoffeeState state_;
  uint32_t stateStartMs_;
  bool startRequested_;
  const char* errorMessage_;

  Timeout pressTimer_;
  Timeout readyTimer_;
  Timeout leverTimer_;
  Timeout brewTimer_;
  Timeout doneTimer_;
};
