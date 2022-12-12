#include "ADS7830.h"

#include <RaspiLib.h>

using namespace PiLib;

#include <iostream>

// I2C default address 0x48


ADS7830::ADS7830(int address)
{
  m_address = address;
}

ADS7830::~ADS7830()
{}
      
ErrorCode ADS7830::Read(int channel, int &value)
{
  value = 0;
  // channel 0 .. 7
  // channel 0 -> 0
  // channel 1 -> 4
  // channel 2 -> 1
  // channel 3 -> 5
  // channel 4 -> 2
  // channel 5 -> 6
  // channel 6 -> 3
  // channel 7 -> 7
  channel = (channel << 2) | (channel >> 1);
  channel = channel & 0x07;

  II2c &iface = RaspiLib::GetInstance().GetI2c();
  ByteBuffer bb;
  int reg = ADS7830_Mode | (channel << 4);  // command byte

  ErrorCode err = iface.ReadRegister_RepeatStart(m_address, reg, bb, 2);  // read 1 byte
  if (err == E_OK)
    value = bb[1];

  return err;
}
