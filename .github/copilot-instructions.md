# Copilot instructions for this repository

## Project focus
- This repo centers on ESP32-S3 firmware in `firmware/`; other top-level folders are mostly placeholders with README stubs.
- The runtime is a non-blocking Arduino loop that coordinates WiFi, HTTP UI, and a coffee-cycle state machine.

## Architecture map (read these first)
- Entry point: `firmware/src/main.cpp` (`setup()` initializes logger + app, `loop()` delegates to app).
- Composition root: `firmware/src/app/AppCoordinator.cpp` wires all modules and owns call order in `loop()`.
- Domain flow: `firmware/src/app/CoffeeStateMachine.*` drives the coffee cycle (`IDLE -> ... -> DONE/ERROR`).
- Hardware adapters: `firmware/src/drivers/*` (`ButtonSim`, `ReadySignal`, `LeverServo`).
- Infra/services: `firmware/src/services/*` (`WifiManager`, `Logger`, `Timeout`).
- UI/API: `firmware/src/ui/WebUi.*` + embedded assets in `firmware/src/ui/WebAssets.h`.

## State and data flow
- UI `POST /api/start` triggers `AppCoordinator` callback -> `CoffeeStateMachine::requestStart()`.
- `CoffeeStateMachine::update()` is polled every loop; transitions are timer/signal driven using `millis()` and `Timeout`.
- UI `GET /api/status` returns JSON built in `AppCoordinator::statusJson()` with `state`, `error`, `states`, optional `message`.
- Keep `stateOrder()` and `stateName()` consistent with UI expectations in `WebAssets.h` (`defaultStates` mirrors machine order).

## Project conventions to preserve
- Prefer constructor injection + references for cross-component wiring (see `AppCoordinator` and `CoffeeStateMachine` constructors).
- Keep logic non-blocking in `loop()`; avoid long waits and avoid adding `delay()` in runtime paths.
- Use `Timeout` + elapsed-time checks instead of ad-hoc timestamp math when adding timed transitions.
- Use `Settings::RuntimeSettings` for tunables and `Pins` namespace constants for GPIO; do not scatter literals.
- Use `Logger::{info,warn,error}` for observability; state transitions are logged in `transitionTo()`.
- Follow existing naming/style: `*_` private members, small focused classes, `.h/.cpp` pairs per module.

## Build, flash, and debug workflow
- Required setup: copy `firmware/secrets.h.example` to `firmware/secrets.h` and set `COFFEE_WIFI_SSID` / `COFFEE_WIFI_PASS`.
- Build firmware: `pio run -d firmware`.
- Upload firmware: `pio run -t upload -d firmware` (matches VS Code task `PIO: Upload`).
- Serial logs: monitor at `115200` baud (`platformio.ini`), useful for WiFi + state transitions.
- Library dependency is managed by PlatformIO: `madhephaestus/ESP32Servo`.

## Change guidance for agents
- When adding a new coffee-cycle step, update all of: `CoffeeState` enum, `update()`, `transitionTo()`, `stateName()`, and (if user-visible) `stateOrder()`.
- If API payload shape changes, update both `AppCoordinator::statusJson()` and UI parsing in `WebAssets.h` (`fetchStatus`).
- For hardware behavior changes, modify corresponding driver class first, then state machine usage.
- There is no established automated test suite yet (`tests/README.md` is a placeholder); validate changes with build + on-device logs.