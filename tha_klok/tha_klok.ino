#include <Arduino_FreeRTOS.h>
#include <WiFiS3.h>

#include "ClockService.h"
#include "NTPService.h"
#include "WiFiService.h"

#include "wifi_credentials.h"
#ifndef WIFI_CREDENTIALS_SSID
  #error "WIFI_CREDENTIALS_SSID was not defined in wifi_credentials.h"
#endif
#ifndef WIFI_CREDENTIALS_PASSWORD
  #error "WIFI_CREDENTIALS_PASSWORD was not defined in wifi_credentials.h"
#endif

ClockService  clockService;
WiFiService   wifiService(WIFI_CREDENTIALS_SSID, WIFI_CREDENTIALS_PASSWORD);
NTPService    ntpService(&wifiService);

void clockTask(void* pvParameters) {
  clockService.taskLoop();
  vTaskDelete(NULL);
}

void wifiTask(void* pvParameters) {
  wifiService.taskLoop();
  vTaskDelete(NULL);
}

void ntpTask(void* pvParameters) {
  ntpService.taskLoop();
  vTaskDelete(NULL);
}

void setup() {
  Serial.begin(115200);

  Serial.println("\n\n\nStarting tha klok...\n");
  delay(1000);  // Wait for the Serial Monitor to connect

  xTaskCreate(wifiTask,   "WiFiTask",   512,  NULL, 2, NULL);
  xTaskCreate(ntpTask,    "NTPTask",    512,  NULL, 2, NULL);
  xTaskCreate(clockTask,  "ClockTask",  256,  NULL, 1, NULL);

  vTaskStartScheduler();  // Starts FreeRTOS
}

void loop() {
  // FreeRTOS handles everything
}
