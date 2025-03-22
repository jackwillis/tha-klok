#pragma once

#include "Logger.h"
#include "WiFiManager.h"

class NTPManager {
public:
  NTPManager(WiFiManager* wifiManager);
  
  void taskLoop();

private:
  bool syncNow();

  WiFiManager* wifi;
  bool firstSyncComplete;
  Logger log;
};
