#include "Logger.h"

SemaphoreHandle_t Logger::logSemaphore = nullptr;

Logger::Logger(const char* tag)
  : tag(tag) {
  if (!logSemaphore) {
    logSemaphore = xSemaphoreCreateBinary();  // 🔑 Create the binary semaphore
    if (logSemaphore) {
      xSemaphoreGive(logSemaphore);  // 🔑 Set it to available
    }
  }
}

void Logger::debug(const String& msg) {
  log("DEBUG", msg);
}

void Logger::info(const String& msg) {
  log("INFO ", msg);
}

void Logger::warn(const String& msg) {
  log("WARN ", msg);
}

void Logger::error(const String& msg) {
  log("ERROR", msg);
}

void Logger::log(const char* level, const String& msg) {
  if (!logSemaphore) {
    return;
  }
  
  // Wait for semaphore to be available
  if (xSemaphoreTake(logSemaphore, portMAX_DELAY) != pdTRUE) {
    return;
  }

  // Format number of seconds since the device has been on
  char timestamp[16];
  float seconds = millis() / 1000.0;
  snprintf(timestamp, sizeof(timestamp), "%10.3f", seconds);
  
  Serial.print(timestamp);
  Serial.print(' ');
  Serial.print(level);
  Serial.print(' ');

  char tagBuf[7];
  snprintf(tagBuf, sizeof(tagBuf), "%6s", tag);
  Serial.print(tagBuf);

  Serial.print(" | ");
  Serial.println(msg);
  
  // Return the semaphore
  xSemaphoreGive(logSemaphore);
}
