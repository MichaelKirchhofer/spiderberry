#pragma once

#include <string>

#include <ErrorCode.h>
#include <IPin.h>

namespace PiLib
{
  class LCD
  {
    public:
      // width 16 or 20, default 16
      // height 1, 2 or 4, default 2
      // any other value will be set to default 
      LCD(int width=16, int height=2);
      ~LCD();

      ErrorCode init(bool nibble, RaspiPinLabel rs, RaspiPinLabel strb, 
                     RaspiPinLabel d0, RaspiPinLabel d1, RaspiPinLabel d2, RaspiPinLabel d3,
                     RaspiPinLabel d4, RaspiPinLabel d5, RaspiPinLabel d6, RaspiPinLabel d7);

      // goto 0;0
      ErrorCode home();
      // clear display
      ErrorCode clear();
      // switch on/off
      ErrorCode display(bool on);
      // switch cursor on/off
      ErrorCode cursor(bool on);
      // switch on blinking of cursor
      ErrorCode blink(bool on);
      // send command
      ErrorCode command(unsigned char cmd);
      // goto position, 0,0 is left top
      ErrorCode position(int x, int y);
      // define character map
      ErrorCode defineChar(int index, unsigned char data[8]);
      // print one char at current location
      ErrorCode putChar(unsigned char data);
      // print string at current position
      ErrorCode putString(const std::string &data);

    private:
      bool m_isInit = false;

      int m_bits = 8;
      // display size
      int m_rows = 0, m_cols = 0;
      // cursor position
      int m_cx = 0, m_cy = 0;

      int m_control = 0;

      IPin *RS_PIN = nullptr;
      IPin *STRB_PIN = nullptr;
      IPin *D0_PIN = nullptr;
      IPin *D1_PIN = nullptr;
      IPin *D2_PIN = nullptr;
      IPin *D3_PIN = nullptr;
      IPin *D4_PIN = nullptr;
      IPin *D5_PIN = nullptr;
      IPin *D6_PIN = nullptr;
      IPin *D7_PIN = nullptr;

      void strobe();
      
      void sendByte(bool RS, unsigned char data);
      void sendNibble(unsigned char data);

      void sendInitSequence();
  };
}