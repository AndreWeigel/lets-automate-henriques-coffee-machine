#include "app/CoffeeStateMachine.h"

#include "services/Logger.h"

CoffeeStateMachine::CoffeeStateMachine(const Settings::RuntimeSettings& settings,
                                       ButtonSim& buttonSim,
                                       ReadySignal& readySignal,
                                       LeverServo& leverServo)
    : settings_(settings),
      buttonSim_(buttonSim),
      readySignal_(readySignal),
      leverServo_(leverServo),
      state_(CoffeeState::IDLE),
      stateStartMs_(0),
      startRequested_(false),
      errorMessage_(nullptr) {}

void CoffeeStateMachine::begin() {
  buttonSim_.begin();
  readySignal_.begin();
  leverServo_.begin();
  transitionTo(CoffeeState::IDLE, millis());
}

void CoffeeStateMachine::requestStart() {
  startRequested_ = true;
}

void CoffeeStateMachine::update() {
  uint32_t nowMs = millis();
  readySignal_.update(nowMs);

  switch (state_) {
    case CoffeeState::IDLE:
      if (startRequested_) {
        startRequested_ = false;
        transitionTo(CoffeeState::PRESS_BUTTON, nowMs);
      }
      break;
    case CoffeeState::PRESS_BUTTON:
      if (pressTimer_.expired(nowMs)) {
        buttonSim_.setPressed(false);
        transitionTo(CoffeeState::WAIT_READY, nowMs);
      }
      break;
    case CoffeeState::WAIT_READY:
      if (readySignal_.isReady(nowMs)) {
        transitionTo(CoffeeState::ACTUATE_LEVER, nowMs);
      } else if (readyTimer_.expired(nowMs)) {
        errorMessage_ = "Ready timeout";
        transitionTo(CoffeeState::ERROR, nowMs);
      }
      break;
    case CoffeeState::ACTUATE_LEVER:
      if (leverTimer_.expired(nowMs)) {
        transitionTo(CoffeeState::BREWING, nowMs);
      }
      break;
    case CoffeeState::BREWING:
      if (brewTimer_.expired(nowMs)) {
        transitionTo(CoffeeState::RETURN_LEVER, nowMs);
      }
      break;
    case CoffeeState::RETURN_LEVER:
      if (leverTimer_.expired(nowMs)) {
        transitionTo(CoffeeState::DONE, nowMs);
      }
      break;
    case CoffeeState::DONE:
      if (doneTimer_.expired(nowMs)) {
        transitionTo(CoffeeState::IDLE, nowMs);
      }
      break;
    case CoffeeState::ERROR:
      if (startRequested_) {
        startRequested_ = false;
        errorMessage_ = nullptr;
        transitionTo(CoffeeState::PRESS_BUTTON, nowMs);
      }
      break;
  }
}

StateSnapshot CoffeeStateMachine::snapshot() const {
  return StateSnapshot{state_, stateName(state_), state_ == CoffeeState::ERROR, errorMessage_};
}

void CoffeeStateMachine::transitionTo(CoffeeState next, uint32_t nowMs) {
  state_ = next;
  stateStartMs_ = nowMs;

  switch (state_) {
    case CoffeeState::IDLE:
      buttonSim_.setPressed(false);
      leverServo_.setRestPosition();
      Logger::info("State: IDLE");
      break;
    case CoffeeState::PRESS_BUTTON:
      buttonSim_.setPressed(true);
      pressTimer_.start(settings_.buttonPressMs, nowMs);
      Logger::info("State: PRESS_BUTTON");
      break;
    case CoffeeState::WAIT_READY:
      readyTimer_.start(settings_.readyTimeoutMs, nowMs);
      Logger::info("State: WAIT_READY");
      break;
    case CoffeeState::ACTUATE_LEVER:
      leverServo_.setBrewPosition();
      leverTimer_.start(settings_.leverMoveMs, nowMs);
      Logger::info("State: ACTUATE_LEVER");
      break;
    case CoffeeState::BREWING:
      brewTimer_.start(settings_.brewTimeMs, nowMs);
      Logger::info("State: BREWING");
      break;
    case CoffeeState::RETURN_LEVER:
      leverServo_.setRestPosition();
      leverTimer_.start(settings_.leverMoveMs, nowMs);
      Logger::info("State: RETURN_LEVER");
      break;
    case CoffeeState::DONE:
      doneTimer_.start(settings_.doneHoldMs, nowMs);
      Logger::info("State: DONE");
      break;
    case CoffeeState::ERROR:
      buttonSim_.setPressed(false);
      leverServo_.setRestPosition();
      Logger::error("State: ERROR");
      break;
  }
}

const char* CoffeeStateMachine::stateName(CoffeeState state) const {
  switch (state) {
    case CoffeeState::IDLE:
      return "IDLE";
    case CoffeeState::PRESS_BUTTON:
      return "PRESS_BUTTON";
    case CoffeeState::WAIT_READY:
      return "WAIT_READY";
    case CoffeeState::ACTUATE_LEVER:
      return "ACTUATE_LEVER";
    case CoffeeState::BREWING:
      return "BREWING";
    case CoffeeState::RETURN_LEVER:
      return "RETURN_LEVER";
    case CoffeeState::DONE:
      return "DONE";
    case CoffeeState::ERROR:
      return "ERROR";
  }
  return "UNKNOWN";
}
