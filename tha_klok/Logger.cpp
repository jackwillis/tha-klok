#include "Logger.h"

SemaphoreHandle_t Logger::logSemaphore = nullptr;

Logger::Logger(const char* tag)
  : _tag(tag) {
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
  log(" INFO", msg);
}

void Logger::warn(const String& msg) {
  log(" WARN", msg);
}

void Logger::error(const String& msg) {
  log("ERROR", msg);
}

void Logger::log(const char* level, const String& msg) {
  if (!logSemaphore || xSemaphoreTake(logSemaphore, portMAX_DELAY) != pdTRUE) {
    return;
  }

  Serial.print(level);
  Serial.print(" [");
  Serial.print(_tag);
  Serial.print("] ");
  Serial.println(msg);
  
  xSemaphoreGive(logSemaphore);
}
