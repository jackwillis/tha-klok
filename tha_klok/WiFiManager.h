#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <WiFiS3.h>
#include "Logger.h"

class WifiManager {
  public:
    WifiManager(const char* ssid, const char* password);

    void connect();
    bool isConnected();
    void printStatus();

  private:
    const char* _ssid;
    const char* _password;
    Logger log;
};

#endif
