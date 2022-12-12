#include <bcm2835.h>

#include "adctest.h"

ADCTest::ADCTest(int address)
{
  m_address = address;
}

ADCTest::~ADCTest()
{}

int ADCTest::Write(int value)
{
  unsigned char buf[2];
  buf[0] = 0x40;  // control character
  buf[1] = value & 0xFF;

  bcm2835_i2c_setSlaveAddress(m_address);
  int reason = bcm2835_i2c_write((char *)buf,2);

  return reason;
}
      
int ADCTest::Read(int channel, int &value)
{
  value = 0;

  unsigned char buf[2] = {0, 0};

  unsigned char reg = (0x40 | channel & 0x03);  // control byte

  bcm2835_i2c_setSlaveAddress(m_address);
  int reason = bcm2835_i2c_read_register_rs((char *)&reg,(char *)buf,1);
  reason = bcm2835_i2c_read_register_rs((char *)&reg,(char *)buf+1,1);

  value = buf[1];
  return reason;
}
