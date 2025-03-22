#pragma once

#include "Logger.h"

class WiFiService {
public:
  WiFiService(const char* ssid, const char* password);
  
  static bool isConnected();

  void taskLoop();

private:
  void connect();
  void printStatus();

  const char* ssid;
  const char* password;
  Logger log;
};
