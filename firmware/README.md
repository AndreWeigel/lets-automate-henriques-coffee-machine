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

UI is embedded in firmware and served on port 80.
