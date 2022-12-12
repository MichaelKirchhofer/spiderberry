#pragma once

#include "II2c.h"

namespace PiLib
{
  class I2C : public II2c
  {
    public:
      I2C();
      virtual ~I2C();

      ErrorCode init();
      ErrorCode shutdown();

      // interface functions

      // -----------------------------------------------------------------------
      // write n bytes to the given address
      virtual ErrorCode WriteByte(int address, uint8_t buf);

      // read n bytes from the given address
      virtual ErrorCode ReadByte(int address, uint8_t &buf);

      // write n bytes to the given register at address
      virtual ErrorCode WriteRegister(int address, int register, const ByteBuffer &buf);

      // read register content from address, repeat start signal without stop
      virtual ErrorCode ReadRegister_RepeatStart(int address, int register, ByteBuffer &buf, int len);

      // write buffer and read answer with repeated start in between
      virtual ErrorCode Write_Read_RepeatStart(int address, const ByteBuffer &sbuf, ByteBuffer &rbuf, int rlen);

      // find all bus participants and provide the address
      virtual ErrorCode Find_Bus_Partners(ByteBuffer &addresses);

      // ------------------------------------------------------------------------
      // set I2C baud rate, default is 100000
      virtual ErrorCode SetBaudRate(uint32_t baudrate);

      // write n bytes to the given address
      virtual ErrorCode Write(int address, const ByteBuffer &buf);

      // read n bytes from the given address
      virtual ErrorCode Read(int address, ByteBuffer &buf, int len);

    private:
      bool m_isInit = false;

      // adds for smbus
      int m_file = -1;
      char m_filename[20] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
      unsigned long m_funcs = 0UL;

  };

}