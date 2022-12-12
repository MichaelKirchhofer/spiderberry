#pragma once

#include "ErrorCode.h"

#include <cstdint>
#include <vector>

typedef std::vector<uint8_t> ByteBuffer;

namespace PiLib
{
  // Raspberry Pi I2C interface
  class II2c
  {
    public:

      // set I2C baud rate, default is 100000
      // virtual ErrorCode SetBaudRate(uint32_t baudrate) = 0;

      // write n bytes to the given address
      virtual ErrorCode WriteByte(int address, uint8_t buf) = 0;

      // read len bytes from the given address
      virtual ErrorCode ReadByte(int address, uint8_t &buf) = 0;

      // write n bytes to the given register at address
      virtual ErrorCode WriteRegister(int address, int register, const ByteBuffer &buf) = 0;

      // read register content from address, repeat start signal without stop
      virtual ErrorCode ReadRegister_RepeatStart(int address, int register, ByteBuffer &buf, int len) = 0;

      // write buffer and read answer with repeated start in between
      // virtual ErrorCode Write_Read_RepeatStart(int address, const ByteBuffer &sbuf, ByteBuffer &rbuf, int rlen) = 0;
  
      // find all bus participants and provide the address
      virtual ErrorCode Find_Bus_Partners(ByteBuffer &addresses) = 0;
  };

};