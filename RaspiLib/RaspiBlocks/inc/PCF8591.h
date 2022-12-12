#pragma once

// PCF8591 ADC/DAC 
// I2C controlled

#include <ErrorCode.h>

namespace PiLib
{
  class PCF8591
  {
    public:
      PCF8591(int address);
      ~PCF8591();

      ErrorCode Write(int value);
      ErrorCode Read(int channel, int &value);

    private:
      int m_address = 0;
  };
}
 