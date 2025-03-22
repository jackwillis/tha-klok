#pragma once

#include "Logger.h"
#include "WiFiService.h"

class NTPService {
public:
  NTPService(WiFiService* wifiService);
  
  void taskLoop();

private:
  bool syncNow();

  WiFiService* wifiService;
  bool firstSyncComplete;
  Logger log;
};
