#pragma once

#include "Logger.h"

class ClockService {
public:
  ClockService();

  void taskLoop();

private:
  Logger log;
};
