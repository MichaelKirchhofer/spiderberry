#include "PCF8591.h"

#include <RaspiLib.h>

using namespace PiLib;

PCF8591::PCF8591(int address)
{
  m_address = address;
}

PCF8591::~PCF8591()
{}

ErrorCode PCF8591::Write(int value)
{
  II2c &iface = RaspiLib::GetInstance().GetI2c();
  ByteBuffer bb;
  bb.emplace_back(value & 0xFF);  // the value to write

  // control register
  ErrorCode err = iface.WriteRegister(m_address, 0x40, bb);
  return err;
}
      
ErrorCode PCF8591::Read(int channel, int &value)
{
  value = 0;

  II2c &iface = RaspiLib::GetInstance().GetI2c();
  ByteBuffer bb;
  int reg = (0x40 | channel & 0x03);  // control byte

  ErrorCode err = iface.ReadRegister_RepeatStart(m_address, reg, bb, 2);  // read 2 bytes, drop first
  if (err == E_OK)
    value = bb[1];

  return err;
}
