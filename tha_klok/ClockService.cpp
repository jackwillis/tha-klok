#include "ClockService.h"
#include <Arduino_FreeRTOS.h>

ClockService::ClockService()
  : log("Clock") {}

void ClockService::taskLoop() {
  while (true) {
    log.debug("Tick.");
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}
