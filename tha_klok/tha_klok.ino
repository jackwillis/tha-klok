#include <Arduino_FreeRTOS.h>
#include <WiFiS3.h>

#include "ClockService.h"
#include "MatrixService.h"
#include "NTPService.h"
#include "RTCManager.h"
#include "WiFiService.h"

#include "wifi_credentials.h"
#ifndef WIFI_CREDENTIALS_SSID
  #error "WIFI_CREDENTIALS_SSID was not defined in wifi_credentials.h"
#endif
#ifndef WIFI_CREDENTIALS_PASSWORD
  #error "WIFI_CREDENTIALS_PASSWORD was not defined in wifi_credentials.h"
#endif

ClockService  clockService;
MatrixService matrixService;
WiFiService   wifiService(WIFI_CREDENTIALS_SSID, WIFI_CREDENTIALS_PASSWORD);
NTPService    ntpService;

void setup() {
  Serial.begin(115200);
  Serial.println("\n\n\nStarting tha klok...\n");

  RTCManager::begin();

  xTaskCreate(clockTask,  "Clock",  256, nullptr, 2, nullptr);
  xTaskCreate(matrixTask, "Matrix", 256, nullptr, 2, nullptr);
  xTaskCreate(wifiTask,   "WiFi",   256, nullptr, 1, nullptr);
  xTaskCreate(ntpTask,    "NTP",    256, nullptr, 1, nullptr);

  vTaskStartScheduler();  // Starts FreeRTOS
}

void loop() {
  // FreeRTOS handles everything
}

void clockTask(void* pvParameters) {
  clockService.taskLoop();
  vTaskDelete(nullptr);
}

void matrixTask(void* pvParameters) {
  matrixService.taskLoop();
  vTaskDelete(nullptr);
}

void wifiTask(void* pvParameters) {
  wifiService.taskLoop();
  vTaskDelete(nullptr);
}

void ntpTask(void* pvParameters) {
  ntpService.taskLoop();
  vTaskDelete(nullptr);
}
