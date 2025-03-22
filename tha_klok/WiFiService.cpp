#include "WiFiService.h"
#include <WiFiS3.h>
#include <Arduino_FreeRTOS.h>

bool hasAttemptedConnection = false;

WiFiService::WiFiService(const char* ssid, const char* password)
  : ssid(ssid), password(password), log("WiFi") {}

void WiFiService::taskLoop() {
  while (true) {
    printStatus();

    if (!isConnected()) {
      connect();
    }

    // Check the connection status every 30 seconds.
    vTaskDelay(pdMS_TO_TICKS(30000));
  }
}

// Returns true only if we have a link connection and a valid IP.
bool WiFiService::isConnected() {
  return hasAttemptedConnection &&
         (WiFi.status() == WL_CONNECTED) &&
         (WiFi.localIP() != IPAddress(0, 0, 0, 0));
}

// Attempt to establish a WiFi connection, waiting for DHCP if necessary.
void WiFiService::connect() {
  log.info("Attempting WiFi connection...");
  WiFi.disconnect();
  WiFi.begin(ssid, password);

  hasAttemptedConnection = true;

  // Wait up to 10 seconds for the link-layer connection (WL_CONNECTED).
  const unsigned long connectTimeout = 10000;
  unsigned long startAttempt = millis();
  bool linkConnected = false;
  
  while ((millis() - startAttempt) < connectTimeout) {
    if (WiFi.status() == WL_CONNECTED) {
      linkConnected = true;
      break;
    }
    log.debug("Waiting for link-layer connection...");
    vTaskDelay(pdMS_TO_TICKS(250));
  }
  
  if (!linkConnected) {
    log.error("WiFi connection attempt timed out (link layer).");
    WiFi.disconnect();
    return;  // Try again next cycle.
  }
  
  // Wait up to another 10 seconds for DHCP to assign a valid IP address.
  const unsigned long dhcpTimeout = 10000;
  startAttempt = millis();
  
  while ((millis() - startAttempt) < dhcpTimeout) {
    IPAddress ip = WiFi.localIP();
    if (ip != IPAddress(0, 0, 0, 0)) {
      log.info("WiFi connected and DHCP assigned an IP address.");
      printStatus();
      return;  // Successful connection.
    }
    log.debug("Waiting for DHCP assignment...");
    vTaskDelay(pdMS_TO_TICKS(250));
  }
  
  // DHCP assignment timed out.
  log.error("DHCP assignment timed out. Restarting connection...");
  WiFi.disconnect();
  vTaskDelay(pdMS_TO_TICKS(500));
}

// Report the current connection status.
void WiFiService::printStatus() {
  if (WiFi.status() == WL_CONNECTED) {
    IPAddress ip = WiFi.localIP();
    if (ip == IPAddress(0, 0, 0, 0)) {
      log.warn(String("Connected to ") + ssid + ", but no IP assigned!");
    } else {
      long rssi = WiFi.RSSI();
      log.debug(String("Connected to ") + ssid + " (RSSI: " + rssi + " dBm)");
      log.debug(String("IP address: ") + ip.toString());
    }
  } else {
    log.info("WiFi not connected.");
  }
}
