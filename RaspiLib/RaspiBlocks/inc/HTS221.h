#pragma once

// HTS221 humidity & temperature sensor
// Raspberry Pi Sense Hat
// I2C controlled

namespace PiLib
{
  // default address for Sense Hat
  const int HTS221_I2C_ADDRESS = 0x5F;
  const int HTS221_ID = 0xBC;

  class HTS221
  {
    public:
      HTS221(int address = HTS221_I2C_ADDRESS);
      ~HTS221();

      // read calibration settings
      bool init();

      double getHumidity();
      double getTemperature();

    private:
      int m_address = 0; 

      // last values
      double m_humidity = 0.;
      double m_temp = 0.;
      bool m_humValid = false;
      bool m_tempValid = false;

      // calibration data
      double m_temp_k = 0.;
      double m_temp_d = 0.;
      double m_hum_k = 0.;
      double m_hum_d = 0.;
      bool m_isInit = false; 

      bool updateData();
  };

}