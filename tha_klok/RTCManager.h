#pragma once

#include <TimeLib.h>

class RTCManager {
public:
  static void begin();

  static void setTimeUTC(time_t epoch);   // Set RTC from NTP
  static time_t nowUTC();                 // Get current UTC time
  static time_t nowLocal();               // Local time for America/Chicago (with DST applied)

  static void setLastNTPSync(time_t t);
  static time_t getLastNTPSync();

private:
  static int getUtcOffset(time_t t);      // Calculates UTC offset for America/Chicago with DST
  static time_t makeFirstDayOfMonth(int year, int month);
};
