#include "ClockManager.h"
#include <Arduino_FreeRTOS.h>

ClockManager::ClockManager()
  : log("Clock") {}

void ClockManager::taskLoop() {
  while (true) {
    log.debug("Tick.");
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}
