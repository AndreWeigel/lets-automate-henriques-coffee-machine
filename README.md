# Let's Automate Henrique's Coffee Machine

This project upgrades Henrique's perfectly good coffee machine into a slightly
overengineered, fully automated caffeine robot. Press one button in a web page —
or hit `POST /api/start` — and an ESP32 presses the machine's button, waits for
it to warm up, pulls the brew lever, holds it, and puts it back. No barista
required.

The machine is never modified destructively: the ESP32 drives the *existing*
controls through a servo and a couple of GPIO lines, so it stays a normal coffee
machine you can still use by hand.

## How it works

The ESP32-S3 interfaces with the machine through three lines:

| Signal | Pin | Role |
|---|---|---|
| **Button sim** | GPIO 4 | Momentarily "presses" the machine's power/brew button |
| **Ready signal** | GPIO 5 | Reads back when the machine reports it's heated and ready |
| **Lever servo** | GPIO 6 | Actuates the brew lever between rest and brew angles |

A brew cycle runs as an explicit state machine:

```
IDLE → PRESS_BUTTON → WAIT_READY → ACTUATE_LEVER → BREWING → RETURN_LEVER → DONE
                          │
                          └─(timeout)─▶ ERROR
```

Every step is time-bounded. `WAIT_READY` has a timeout so a machine that never
signals ready drops into `ERROR` instead of hanging, and the lever always returns
to rest before the cycle ends. Timings and servo angles live in one place
([`firmware/src/config/Settings.h`](firmware/src/config/Settings.h)) — brew time,
ready timeout, lever travel, rest/brew angles, and pulse widths are all tunable
without touching logic.

## Control it

On boot the firmware comes up in **AP+STA** mode and serves an embedded web UI on
port 80:

- Joins your WiFi (STA) and advertises itself over mDNS at `http://coffee.local`
- Also broadcasts its own access point as a fallback (`http://192.168.4.1`)

REST API:

- `POST /api/start` — begin a brew cycle
- `GET  /api/status` — current state snapshot

A compile-time `COFFEE_TEST_MODE` flag exposes extra endpoints for exercising each
step (button, ready, lever) in isolation — handy when bringing up the hardware.
See [`firmware/README.md`](firmware/README.md) for the full endpoint list and
build flags.

## Build & flash

```bash
cp firmware/secrets.h.example firmware/secrets.h   # add your WiFi credentials
pio run -t upload -d firmware                       # env: esp32-s3-devkitc-1
```

Built with [PlatformIO](https://platformio.org/) on the Arduino framework. The
only external dependency is `ESP32Servo`. Full details in
[`firmware/README.md`](firmware/README.md).

## Repository layout

```
firmware/     ESP32-S3 firmware (PlatformIO)
  src/
    app/        AppCoordinator + CoffeeStateMachine
    config/     Pins.h, Settings.h
    drivers/    ButtonSim, ReadySignal, LeverServo
    services/   WifiManager, Logger, Timeout
    ui/         embedded web UI + WebAssets
hardware/     schematics, wiring, power & optocoupler notes
mechanical/   servo mount + lever linkage CAD
docs/         system overview, safety & assembly notes
tests/        test plans and verification checklists
tools/        helper scripts
```

## Status

Firmware is functional: full brew state machine, WiFi + mDNS, embedded web UI,
REST API, and a compile-time test harness. Hardware, mechanical, and docs folders
are being filled in as the physical build progresses.

## License

MIT — see [LICENSE](LICENSE).
