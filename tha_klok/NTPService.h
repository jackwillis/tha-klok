#pragma once

#include "WiFiService.h"
#include "Logger.h"

class NTPService {
public:
  NTPService();

  void taskLoop();

private:
  bool firstSyncComplete;
  Logger log;

  bool syncNow();
};
