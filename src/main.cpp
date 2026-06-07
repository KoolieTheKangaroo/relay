#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <LittleFS.h>

#if __has_include("secrets.h")
#include "secrets.h"
#else
#define WIFI_SSID "CHANGE_ME"
#define WIFI_PASSWORD "CHANGE_ME"
#endif

namespace {
constexpr uint8_t kRelayCount = 6;
constexpr int8_t kRelayPins[kRelayCount] = {2, 26, 27, -1, -1, -1};

WebServer server(80);
bool relayStates[kRelayCount] = {false, false, false, false, false, false};

bool isRelayConfigured(const uint8_t relayId) {
  return relayId < kRelayCount && kRelayPins[relayId] >= 0;
}

void writeRelay(const uint8_t relayId, const bool on) {
  if (!isRelayConfigured(relayId)) {
    return;
  }

  relayStates[relayId] = on;
  digitalWrite(static_cast<uint8_t>(kRelayPins[relayId]), on ? HIGH : LOW);
}

bool parseRelayId(const String &idParam, uint8_t &relayId) {
  if (idParam.isEmpty()) {
    return false;
  }

  const int relayNumber = idParam.toInt();
  if (relayNumber < 1 || relayNumber > kRelayCount) {
    return false;
  }

  relayId = static_cast<uint8_t>(relayNumber - 1);
  return true;
}

void sendJson(const int code, const String &json) {
  server.send(code, "application/json", json);
}

void handleSetRelay(const bool on) {
  uint8_t relayId = 0;
  if (!parseRelayId(server.arg("id"), relayId)) {
    sendJson(400, "{\"error\":\"invalid relay id\"}");
    return;
  }

  if (!isRelayConfigured(relayId)) {
    sendJson(400, "{\"error\":\"relay not configured\"}");
    return;
  }

  writeRelay(relayId, on);
  sendJson(200, "{\"ok\":true,\"relay\":" + String(relayId + 1) + ",\"state\":\"" + (on ? "on" : "off") + "\"}");
}

String statesJson() {
  String json = "{\"relays\":[";
  json.reserve(220);
  for (uint8_t i = 0; i < kRelayCount; ++i) {
    if (i > 0) {
      json += ",";
    }
    json += "{\"id\":" + String(i + 1) + ",\"configured\":" + String(isRelayConfigured(i) ? "true" : "false") +
            ",\"state\":" + String(relayStates[i] ? "true" : "false") + "}";
  }
  json += "]}";
  return json;
}

void handleState() {
  sendJson(200, statesJson());
}

void serveIndex() {
  File file = LittleFS.open("/index.html", "r");
  if (!file) {
    server.send(500, "text/plain", "Missing /index.html in LittleFS");
    return;
  }

  server.streamFile(file, "text/html");
  file.close();
}

void connectWifi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  constexpr uint32_t kWifiTimeoutMs = 15000;
  const uint32_t startMs = millis();
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED && (millis() - startMs) < kWifiTimeoutMs) {
    delay(250);
    Serial.print('.');
  }
  Serial.println();
  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("Connected. IP: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("Wi-Fi timeout, running without network connectivity");
  }
}

void setupRelays() {
  for (uint8_t i = 0; i < kRelayCount; ++i) {
    if (!isRelayConfigured(i)) {
      continue;
    }

    pinMode(static_cast<uint8_t>(kRelayPins[i]), OUTPUT);
    writeRelay(i, false);
  }
}
}  // namespace

void setup() {
  Serial.begin(115200);

  if (!LittleFS.begin(true)) {
    Serial.println("LittleFS init failed");
    while (true) {
      delay(1000);
    }
  }

  setupRelays();
  connectWifi();

  server.on("/", HTTP_GET, serveIndex);
  server.on("/api/relay/on", HTTP_POST, []() { handleSetRelay(true); });
  server.on("/api/relay/off", HTTP_POST, []() { handleSetRelay(false); });
  server.on("/api/relay/state", HTTP_GET, handleState);
  server.begin();

  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}
