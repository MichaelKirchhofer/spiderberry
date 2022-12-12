#pragma once

#include <string>

// PCF8563 Realtime Clock
// I2C controlled

#include <ErrorCode.h>

namespace PiLib
{
  // first define buffer for exchanging time info
  struct TimeInfo
  {
    int second = 0;
    int minute = 0;
    int hour = 0;
    int day = 0;
    int weekday = 0;
    int month = 0;
    int year = 0;
  };

  class PCF8563
  {
    public:
      PCF8563(int address);
      ~PCF8563();

      ErrorCode SetTime(const TimeInfo &ti);
      ErrorCode ReadTime(TimeInfo &ti);

      static std::string ToString(const TimeInfo &ti);

    private:
      int m_address = 0;
      const int SecReg = 0x02;  // register for seconds and more
  };

}