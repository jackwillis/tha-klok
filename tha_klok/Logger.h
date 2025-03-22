#pragma once

#include <Arduino.h>
#include <Arduino_FreeRTOS.h>

class Logger {
public:
  Logger(const char* tag);

  void debug(const String& msg);
  void info(const String& msg);
  void warn(const String& msg);
  void error(const String& msg);

private:
  const char* _tag;
  void log(const char* level, const String& msg);

  static SemaphoreHandle_t logSemaphore;
};
