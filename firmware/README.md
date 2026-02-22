# Firmware

ESP32-S3 (Arduino/PlatformIO) firmware for Henrique's coffee machine automation.

## Build and Flash

1. Copy [firmware/secrets.h.example](secrets.h.example) to firmware/secrets.h and update WiFi credentials.
2. Open the firmware folder in PlatformIO.
3. Build and flash the env `esp32-s3-devkitc-1`.

## Structure

```
firmware/
	platformio.ini
	secrets.h.example
	src/
		app/
			AppCoordinator.*
			CoffeeStateMachine.*
		config/
			Pins.h
			Settings.h
		drivers/
			ButtonSim.*
			ReadySignal.*
			LeverServo.*
		services/
			Logger.*
			Timeout.h
			WifiManager.*
		ui/
			WebAssets.h
			WebUi.*
		main.cpp
```

## State Machine

States:
- IDLE
- PRESS_BUTTON
- WAIT_READY (with timeout)
- ACTUATE_LEVER
- BREWING
- RETURN_LEVER
- DONE
- ERROR

## Configuration

- GPIO pins in [firmware/src/config/Pins.h](src/config/Pins.h)
- Timing and servo settings in [firmware/src/config/Settings.h](src/config/Settings.h)
- WiFi STA credentials in firmware/secrets.h (see template)

## Web UI

Endpoints:
- POST /api/start
- GET /api/status

When built with test mode enabled (see below), additional endpoints are exposed:
- POST /api/test/button-press
- GET /api/test/ready
- POST /api/test/lever-brew
- POST /api/test/lever-rest
- POST /api/test/state-reset

UI is embedded in firmware and served on port 80.

## Test Mode (Compile-Time)

`COFFEE_TEST_MODE` is a compile-time feature flag (not runtime).

- Build with test mode:
	- `PLATFORMIO_BUILD_FLAGS="-DCOFFEE_TEST_MODE" pio run -d firmware`
- Upload with test mode:
	- `PLATFORMIO_BUILD_FLAGS="-DCOFFEE_TEST_MODE" pio run -t upload -d firmware`

When enabled:
- Web UI shows a minimal test panel for independent step testing.
- Test endpoints above are registered.

When disabled:
- Test endpoints are not registered.
- Test panel is not rendered.
