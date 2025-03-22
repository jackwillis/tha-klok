#pragma once
#include "Logger.h"

class ClockManager {
public:
  ClockManager();

  void taskLoop();

private:
  Logger log;
};
