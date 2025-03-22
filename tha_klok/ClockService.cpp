#include "ClockService.h"
#include <Arduino_FreeRTOS.h>
#include "RTCManager.h"

ClockService::ClockService()
  : log("Clock") {}

void ClockService::taskLoop() {
  int tickCount = 0;

  while (true) {
    time_t now = RTCManager::nowLocal();

    char timestamp[9];
    int normHour = hour(now);
    if (normHour == 0) {
      normHour = 12;
    }
    snprintf(timestamp, sizeof(timestamp), "%02d:%02d:%02d",
      normHour, minute(now), second(now));

    log.info(String("Tick: ") + timestamp);

    // // Every 10 ticks (~10 seconds), print memory stats
    // if (++tickCount % 10 == 0) {
    //   size_t heapFree = xPortGetFreeHeapSize();
    //   log.debug(String("Free heap: ") + heapFree + " bytes");
    // }

    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}
