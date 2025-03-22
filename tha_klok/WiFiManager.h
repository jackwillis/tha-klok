#pragma once

#include "Logger.h"

class WiFiManager {
public:
  WiFiManager(const char* ssid, const char* password);
  
  void taskLoop();
  bool isConnected() const;

private:
  static void taskWrapper(void* instance);
  void connect();
  void printStatus();

  const char* ssid;
  const char* password;
  Logger log;
};
