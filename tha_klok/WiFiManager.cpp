#include "WifiManager.h"

WifiManager::WifiManager(const char* ssid, const char* password)
  : _ssid(ssid), _password(password), log("WiFiManager") {}

void WifiManager::connect() {
  log.info(String("Connecting to ") + _ssid);

  WiFi.begin(_ssid, _password);

  unsigned long start = millis();
  const unsigned long timeout = 15000;

  while ((WiFi.status() != WL_CONNECTED || WiFi.localIP() == INADDR_NONE) &&
         millis() - start < timeout) {
    //Serial.print(".");  // Visual connection progress
    delay(500);
  }

  Serial.println();

  if (isConnected()) {
    log.info("Connected!");
    printStatus();
  } else {
    log.error("Failed to connect.");
  }
}

bool WifiManager::isConnected() {
  return (WiFi.status() == WL_CONNECTED && WiFi.localIP() != INADDR_NONE);
}

void WifiManager::printStatus() {
  log.info(String("IP Address: ") + WiFi.localIP().toString());

  uint8_t mac[6];
  WiFi.macAddress(mac);

  String macStr;
  for (int i = 0; i < 6; ++i) {
    if (mac[i] < 16) macStr += "0";
    macStr += String(mac[i], HEX);
    if (i < 5) macStr += ":";
  }

  log.info(String("MAC Address: ") + macStr);
  log.info(String("Signal strength (RSSI): ") + String(WiFi.RSSI()) + " dBm");
}
