#include <Arduino_FreeRTOS.h>
#include <WiFiS3.h>

#include "Logger.h"
#include "WifiManager.h"

#include "wifi_credentials.h"
#ifndef WIFI_CREDENTIALS_SSID
  #error "WIFI_CREDENTIALS_SSID was not defined in wifi_credentials.h"
#endif
#ifndef WIFI_CREDENTIALS_PASSWORD
  #error "WIFI_CREDENTIALS_PASSWORD was not defined in wifi_credentials.h"
#endif

// Instantiate WifiManager and loggers
WifiManager wifi(WIFI_CREDENTIALS_SSID, WIFI_CREDENTIALS_PASSWORD);
Logger wifiLog("WiFi Task");
Logger ntpLog("NTP Task");
Logger clockLog("Clock Task");
Logger ntpSyncLog("syncFromNTP");

// Shared WiFi connection flag
volatile bool wifiReady = false;

void wifiTask(void* pvParameters) {
  while (true) {
    if (!wifi.isConnected()) {
      wifiLog.info("WiFi not connected — attempting to connect...");
      wifi.connect();
      wifiReady = wifi.isConnected();
    }

    vTaskDelay(pdMS_TO_TICKS(30000));  // Recheck every 30 sec
  }
}

bool syncFromNTP() {
  ntpSyncLog.info("Pretending to sync NTP...");
  delay(100);
  return true;  // Simulate success
}

void ntpTask(void* pvParameters) {
  bool firstSyncComplete = false;

  while (true) {
    if (!wifiReady) {
      ntpLog.info("Waiting for WiFi...");
      vTaskDelay(pdMS_TO_TICKS(5000));
      continue;
    }

    if (!firstSyncComplete) {
      ntpLog.info("Trying initial NTP sync...");
      bool success = syncFromNTP();

      if (success) {
        ntpLog.info("Initial sync complete!");
        firstSyncComplete = true;
      } else {
        ntpLog.error("Initial sync failed. Retrying...");
        vTaskDelay(pdMS_TO_TICKS(10000));
      }
    } else {
      ntpLog.info("Hourly NTP sync...");
      syncFromNTP();  // Not critical if it fails
      vTaskDelay(pdMS_TO_TICKS(3600000));  // One hour
    }
  }
}

void clockTask(void* pvParameters) {
  while (true) {
    clockLog.info("Tick.");
    vTaskDelay(pdMS_TO_TICKS(1000));  // Every 1 second
  }
}

void setup() {
  Serial.begin(115200);

  Serial.println("\n\n\nStarting tha klok...\n");

  delay(1000);

  xTaskCreate(wifiTask,   "WiFiTask", 512, NULL, 2, NULL);
  xTaskCreate(ntpTask,    "NTPTsk",   512, NULL, 1, NULL);
  xTaskCreate(clockTask,  "ClockTsk", 256, NULL, 1, NULL);

  vTaskStartScheduler();  // Starts FreeRTOS
}

void loop() {
  // FreeRTOS handles everything
}
