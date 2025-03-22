#include "MatrixService.h"
#include <ArduinoGraphics.h>
#include <Arduino_LED_Matrix.h>
#include <Arduino_FreeRTOS.h>
#include "RTCManager.h"
#include "WiFiService.h"

MatrixService::MatrixService()
  : log("Matrix") {}

void MatrixService::taskLoop() {
  if (matrix.begin()) {
    log.info("LED Matrix initialized");
  } else {
    log.error("LED Matrix could not be initialized.");
    return;
  }

  while (true) {
    update();
    vTaskDelay(pdMS_TO_TICKS(250));  // check for updates 4x/second
  }
}

void MatrixService::update() {
  time_t now = RTCManager::nowLocal();
  time_t lastSync = RTCManager::getLastNTPSync();
  const time_t oneHour = 60 * 60;

  if (!WiFiService::isConnected()) {
    matrix.loadFrame(LEDMATRIX_CLOUD_WIFI);
  } 
  else if (!lastSync) {
    // No NTP sync has ever happened
    matrix.loadFrame(LEDMATRIX_EMOJI_BASIC);
  }
  else if ((now < 1735689600) || (now - lastSync > oneHour)) {
    // Time is before Jan 1, 2025 00:00:00 UTC (fallback for bad RTC)
    // OR it's been over an hour since last NTP sync
    matrix.loadFrame(LEDMATRIX_EMOJI_SAD);
  } 
  else {
    // All good — synced and recent
    matrix.loadFrame(LEDMATRIX_EMOJI_HAPPY);
  }
}
