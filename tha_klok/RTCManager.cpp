#include "RTCManager.h"
#include <RTC.h>

static time_t lastNTPSyncTime = 0;

// UTC−6 (CST, America/Chicago) in seconds
static const int CST_OFFSET = -6 * 3600;
static const int DST_OFFSET = 3600; // 1 hour DST adjustment

void RTCManager::begin() {
    RTC.begin();
}

void RTCManager::setTimeUTC(time_t epoch) {
    RTCTime rtcTime(epoch);
    RTC.setTime(rtcTime);
}

time_t RTCManager::nowUTC() {
    RTCTime rtcTime;
    if (!RTC.getTime(rtcTime)) {
        return 0;
    }
    
    tmElements_t tm;
    tm.Year = rtcTime.getYear() - 1970;
    tm.Month = (uint8_t)rtcTime.getMonth() + 1;
    tm.Day = rtcTime.getDayOfMonth();
    tm.Hour = rtcTime.getHour();
    tm.Minute = rtcTime.getMinutes();
    tm.Second = rtcTime.getSeconds();
    
    return makeTime(tm);
}

time_t RTCManager::nowLocal() {
    time_t utc = nowUTC();
    return utc + getUtcOffset(utc);
}

void RTCManager::setLastNTPSync(time_t t) {
  lastNTPSyncTime = t;
}
  
time_t RTCManager::getLastNTPSync() {
  return lastNTPSyncTime;
}

// Determines if the given UTC time falls within DST for America/Chicago timezone
// DST: Second Sunday in March 2:00 AM to First Sunday in November 2:00 AM (local time)
int RTCManager::getUtcOffset(time_t utcTime) {
    // Extract date components from UTC time
    tmElements_t tm;
    breakTime(utcTime, tm);
    int year = tm.Year + 1970;
    int month = tm.Month;
    
    // Simple cases: Dec-Feb is never DST, Apr-Oct is always DST
    if (month <= 2 || month == 12) {
        return CST_OFFSET; // Winter months, no DST
    }
    if (month >= 4 && month <= 10) {
        return CST_OFFSET + DST_OFFSET; // Summer months, DST in effect
    }
    
    // Calculate day of first Sunday in the month (1-7)
    time_t firstDay = makeFirstDayOfMonth(year, month);
    int firstSunday = 1;
    while (weekday(firstDay) != 1) { // 1 = Sunday
        firstDay += SECS_PER_DAY;
        firstSunday++;
    }
    
    // For March: DST starts on second Sunday at 2:00 AM local time
    if (month == 3) {
        int dstStartDay = firstSunday + 7; // Second Sunday
        
        // Convert current day/hour to local standard time
        int localDay = day(utcTime + CST_OFFSET);
        int localHour = hour(utcTime + CST_OFFSET);
        
        // Before second Sunday = no DST
        if (localDay < dstStartDay) {
            return CST_OFFSET;
        }
        // On second Sunday, before 2:00 AM = no DST
        if (localDay == dstStartDay && localHour < 2) {
            return CST_OFFSET;
        }
        // Otherwise, DST is in effect
        return CST_OFFSET + DST_OFFSET;
    }
    
    // For November: DST ends on first Sunday at 2:00 AM local time
    if (month == 11) {
        int dstEndDay = firstSunday; // First Sunday
        
        // Convert current day/hour to local DST time
        int localDay = day(utcTime + CST_OFFSET + DST_OFFSET);
        int localHour = hour(utcTime + CST_OFFSET + DST_OFFSET);
        
        // Before first Sunday = DST
        if (localDay < dstEndDay) {
            return CST_OFFSET + DST_OFFSET;
        }
        // On first Sunday, before 2:00 AM = DST
        if (localDay == dstEndDay && localHour < 2) {
            return CST_OFFSET + DST_OFFSET;
        }
        // Otherwise, no DST
        return CST_OFFSET;
    }
    
    // Should never reach here
    return CST_OFFSET;
}

// Helper function to create time_t for first day of a month
time_t RTCManager::makeFirstDayOfMonth(int year, int month) {
    tmElements_t temp;
    temp.Second = 0;
    temp.Minute = 0;
    temp.Hour = 0;
    temp.Day = 1;
    temp.Month = month;
    temp.Year = year - 1970;
    return makeTime(temp);
}
