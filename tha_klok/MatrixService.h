#pragma once

#include <ArduinoGraphics.h>
#include <Arduino_LED_Matrix.h>
#include "Logger.h"

class MatrixService {
public:
  MatrixService();
  void taskLoop();

private:
  ArduinoLEDMatrix matrix;
  Logger log;

  void update();
};
