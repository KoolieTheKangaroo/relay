# relay
ESP32 Wroom controlling a 6 Relay module.

## Project layout
- PlatformIO project (Arduino + ESP32)
- LittleFS web UI in `/data/index.html`
- Relay firmware in `/src/main.cpp`
- Copy `/include/secrets.h.example` to `/include/secrets.h`

## Initial relay mapping
- Relay 1 -> GPIO 2
- Relay 2 -> GPIO 26
- Relay 3 -> GPIO 27
- Relay 4-6 reserved for later (`-1` placeholder)

## API
- `POST /api/relay/on?id=<1..6>`
- `POST /api/relay/off?id=<1..6>`
- `GET /api/relay/state`

## Notes
- UI on `/` provides 3 relay control buttons.
- Update `APP_VERSION` in `platformio.ini` when bumping versions.
