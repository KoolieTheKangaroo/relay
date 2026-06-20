# relay

ESP32 Wroom controlling a 6 Relay module.

## Project layout

- PlatformIO project (Arduino + ESP32)
- LittleFS web UI in `/data/index.html`
- Relay firmware in `/src/main.cpp`
- Copy `/include/secrets.h.example` to `/include/secrets.h`
- OTA upload is configured for `relay.local`

## Initial relay mapping

- Relay 0 -> GPIO 2
- Relay 1 -> GPIO 26
- Relay 2 -> GPIO 27
- Relay 3-5 reserved for later (`-1` placeholder)

## API

- `POST /api/relay/on` with a JSON number body `0..5`
- `POST /api/relay/off` with a JSON number body `0..5`
- `GET /api/relay/state`

## Wi-Fi flashing

- Build and upload LittleFS once with USB if needed, then use OTA uploads over Wi-Fi.
- Default PlatformIO environment (`esp32dev`) is USB upload.
- OTA upload is in the `esp32dev-ota` environment and targets `relay.local`.
- If you define `OTA_PASSWORD` in `include/secrets.h`, also pass the same auth when uploading.

### Upload commands

- USB firmware upload: `pio run -e esp32dev -t upload`
- USB LittleFS upload: `pio run -e esp32dev -t uploadfs`
- OTA firmware upload: `pio run -e esp32dev-ota -t upload`

## Notes

- UI on `/` provides control buttons for relays `0..2`.
- Update `APP_VERSION` in `platformio.ini` when bumping versions.
