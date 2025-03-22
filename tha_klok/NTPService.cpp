#include "NTPService.h"
#include "RTCManager.h"

#include <Arduino_FreeRTOS.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

WiFiUDP udp;
NTPClient ntp(udp, "pool.ntp.org", 0, 60000);  // No offset, refresh every 60s

NTPService::NTPService()
  : firstSyncComplete(false), log("NTP") {}

void NTPService::taskLoop() {
  while (true) {
    if (!WiFiService::isConnected()) {
      log.info("Waiting for WiFi...");
      vTaskDelay(pdMS_TO_TICKS(5000));
      continue;
    }

    if (!firstSyncComplete) {
      log.info("Trying initial NTP sync...");
      if (syncNow()) {
        log.info("Initial sync complete!");
        firstSyncComplete = true;
        vTaskDelay(pdMS_TO_TICKS(3600000));  // Wait an hour
      } else {
        log.error("Initial sync failed. Retrying...");
        vTaskDelay(pdMS_TO_TICKS(10000));
      }
    } else {
      log.info("Hourly NTP sync...");
      syncNow();  // Not critical if it fails
      vTaskDelay(pdMS_TO_TICKS(3600000));  // Every hour
    }
  }
}

bool NTPService::syncNow() {
  log.debug("Starting NTP sync...");

  ntp.begin();

  if (!ntp.forceUpdate()) {
    log.error("NTP update failed.");
    return false;
  }

  // Success!

  unsigned long epoch = ntp.getEpochTime();
  RTCManager::setTimeUTC(epoch);
  RTCManager::setLastNTPSync(epoch);

  char timestamp[36];
  tmElements_t tm;
  breakTime(epoch, tm);
  snprintf(timestamp, sizeof(timestamp),
           "%04d-%02d-%02d %2d:%02d:%02d UTC",
           tmYearToCalendar(tm.Year),
           tm.Month,
           tm.Day,
           tm.Hour,
           tm.Minute,
           tm.Second);

  log.debug(String("NTP time: ") + epoch);
  log.info(String("NTP time: ") + timestamp);

  return true;
}
