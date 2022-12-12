#pragma once

// LCD I2C  
// I2C controlled

#include <ErrorCode.h>
#include <string>

const int DefaultAddress = 0x27;

namespace PiLib
{
  class LCD_I2C
  {
    public:
      LCD_I2C(int address = DefaultAddress);
      ~LCD_I2C();

      ErrorCode Init();
      ErrorCode Clear();

      ErrorCode Write(int x, int y, std::string text);

    private:

      ErrorCode SendCommand(int command);
      ErrorCode SendData(int data);
      ErrorCode WriteWord(int word);

      int m_address = 0;
  };
}
 