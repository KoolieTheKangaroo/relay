# relay

ESP32 Wroom controlling a 6 Relay module.

## Project layout

- PlatformIO project (Arduino + ESP32)
- LittleFS web UI in `/data/index.html`
- Relay firmware in `/src/main.cpp`
- Copy `/include/secrets.h.example` to `/include/secrets.h`
- OTA upload is configured for `relay.local`

## Initial relay mapping

- Relay 1 -> GPIO 2
- Relay 2 -> GPIO 26
- Relay 3 -> GPIO 27
- Relay 4-6 reserved for later (`-1` placeholder)

## API

- `POST /api/relay/on` with a JSON number body `0..5`
- `POST /api/relay/off` with a JSON number body `0..5`
- `GET /api/relay/state`

## Wi-Fi flashing

- Build and upload LittleFS once with USB if needed, then use OTA uploads over Wi-Fi.
- PlatformIO is configured for `espota` uploads to `relay.local`.
- If you define `OTA_PASSWORD` in `include/secrets.h`, also pass the same auth when uploading.

## Notes

- UI on `/` provides 3 relay control buttons.
- Update `APP_VERSION` in `platformio.ini` when bumping versions.
