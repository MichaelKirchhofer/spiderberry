#pragma once

// ADS7830 ADC 
// I2C controlled

#include <ErrorCode.h>

namespace PiLib
{
  class ADS7830
  {
    const int ADS7830_Mode = 0x84;  // single ended, no auto run

    public:
      ADS7830(int address);
      ~ADS7830();

      ErrorCode Read(int channel, int &value);

    private:
      int m_address = 0;
  };
}
 