#pragma once

// LPS25H air pressure sensor
// Raspberry Pi Sense Hat
// I2C controlled

namespace PiLib
{
  // default address for Sense Hat
  const int LPS25H_I2C_ADDRESS0 = 0x5C;
  const int LPS25H_I2C_ADDRESS1 = 0x5D;
  const int LPS25H_ID = 0xBD;

  class LPS25H
  {
    public:
      LPS25H(int address = LPS25H_I2C_ADDRESS0);
      ~LPS25H();

      // init device
      bool init();

      double getPressure();
      double getTemperature();

    private:
      int m_address = 0; 

      // last values
      double m_pressure = 0.;
      double m_temperature = 0.;
      bool m_pValid = false;
      bool m_tValid = false;

      bool m_isInit = false; 

      bool updateData();
  };
}
