#include "NTPService.h"
#include <Arduino_FreeRTOS.h>

NTPService::NTPService(WiFiService* wifiService)
  : wifiService(wifiService), firstSyncComplete(false), log("NTP  ") {}

// The main task loop for NTP sync
void NTPService::taskLoop() {
  while (true) {
    // If WiFi is not connected, wait 5 seconds before rechecking.
    if (!wifiService->isConnected()) {
      log.info("Waiting for WiFi...");
      vTaskDelay(pdMS_TO_TICKS(5000));
      continue;
    }

    // First sync attempt.
    if (!firstSyncComplete) {
      log.info("Trying initial NTP sync...");
      bool success = syncNow();
      if (success) {
        log.info("Initial sync complete!");
        firstSyncComplete = true;
      } else {
        log.error("Initial sync failed. Retrying...");
        vTaskDelay(pdMS_TO_TICKS(10000));
      }
    }
    // Subsequent hourly syncs.
    else {
      log.info("Hourly NTP sync...");
      syncNow();  // Not critical if it fails
      vTaskDelay(pdMS_TO_TICKS(3600000));  // One hour delay
    }
  }
}

// Simulate an NTP sync (production code would perform actual NTP network calls)
bool NTPService::syncNow() {
  log.info("Pretending to sync NTP...");
  delay(100);  // Simulated network delay
  return true; // Simulate success
}
