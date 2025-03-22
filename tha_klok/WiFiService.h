#pragma once

#include "Logger.h"

class WiFiService {
public:
  WiFiService(const char* ssid, const char* password);
  
  void taskLoop();
  bool isConnected() const;

private:
  void connect();
  void printStatus();

  const char* ssid;
  const char* password;
  Logger log;
};
