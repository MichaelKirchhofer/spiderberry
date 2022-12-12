#include "I2C.h"

#include "bcm2835.h"

using namespace PiLib;

I2C::I2C()
{}

I2C::~I2C()
{}

ErrorCode I2C::init()
{
  int res = bcm2835_i2c_begin();
  m_isInit = (res != 0);

  if (m_isInit)
    // run at 100 kHz
    bcm2835_i2c_setClockDivider(BCM2835_I2C_CLOCK_DIVIDER_2500);

  return (m_isInit) ? E_OK : E_I2C_NOT_INITIALIZED;
}

ErrorCode I2C::shutdown()
{
  bcm2835_i2c_end(); 

  return E_OK;
}

// interface functions
// set I2C baud rate, default is 100000
ErrorCode I2C::SetBaudRate(uint32_t baudrate)
{
  if (! m_isInit) return E_I2C_NOT_INITIALIZED;

  bcm2835_i2c_set_baudrate(baudrate);


  return E_OK;
}

// write n bytes to the given address
ErrorCode I2C::Write(int address, const ByteBuffer &buf)
{
  if (! m_isInit) return E_I2C_NOT_INITIALIZED;

  bcm2835_i2c_setSlaveAddress(address);

  uint32_t len = buf.size();

  uint8_t ret = bcm2835_i2c_write((const char *)buf.data(), len);

  if (ret & BCM2835_I2C_REASON_ERROR_DATA) ret = E_I2C_ERROR_DATA;
  else if (ret & BCM2835_I2C_REASON_ERROR_NACK) ret = E_I2C_ERROR_NACK;
  else if (ret & BCM2835_I2C_REASON_ERROR_CLKT) ret = E_I2C_ERROR_CLKT;

  return (ErrorCode) ret;
}

// read n bytes from the given address
ErrorCode I2C::Read(int address, ByteBuffer &buf, int len)
{
  buf.clear();

  if (! m_isInit) return E_I2C_NOT_INITIALIZED;

  bcm2835_i2c_setSlaveAddress(address);

  buf.reserve(len);

  uint8_t ret = bcm2835_i2c_read((char *)buf.data(), len);

  if (ret & BCM2835_I2C_REASON_ERROR_DATA) ret = E_I2C_ERROR_DATA;
  else if (ret & BCM2835_I2C_REASON_ERROR_NACK) ret = E_I2C_ERROR_NACK;
  else if (ret & BCM2835_I2C_REASON_ERROR_CLKT) ret = E_I2C_ERROR_CLKT;

  return (ErrorCode) ret;
}

// read register content from address, repeat start signal without stop
ErrorCode I2C::ReadRegister_RepeatStart(int address, int reg, ByteBuffer &buf, int len)
{
  buf.clear();

  if (! m_isInit) return E_I2C_NOT_INITIALIZED;

  bcm2835_i2c_setSlaveAddress(address);

  buf.reserve(len);
  char laddress = (char) reg;

  uint8_t ret = bcm2835_i2c_read_register_rs(&laddress, (char *)buf.data(), len);

  if (ret & BCM2835_I2C_REASON_ERROR_DATA) ret = E_I2C_ERROR_DATA;
  else if (ret & BCM2835_I2C_REASON_ERROR_NACK) ret = E_I2C_ERROR_NACK;
  else if (ret & BCM2835_I2C_REASON_ERROR_CLKT) ret = E_I2C_ERROR_CLKT;

  return (ErrorCode) ret;
}

// write buffer and read answer with repeated start in between
ErrorCode I2C::Write_Read_RepeatStart(int address, const ByteBuffer &sbuf, ByteBuffer &rbuf, int rlen)
{
  rbuf.clear();

  if (! m_isInit) return E_I2C_NOT_INITIALIZED;

  bcm2835_i2c_setSlaveAddress(address);
  
  rbuf.reserve(rlen);

  uint8_t ret = bcm2835_i2c_write_read_rs((char *)sbuf.data(), sbuf.size(), (char *)rbuf.data(), rlen);

  if (ret & BCM2835_I2C_REASON_ERROR_DATA) ret = E_I2C_ERROR_DATA;
  else if (ret & BCM2835_I2C_REASON_ERROR_NACK) ret = E_I2C_ERROR_NACK;
  else if (ret & BCM2835_I2C_REASON_ERROR_CLKT) ret = E_I2C_ERROR_CLKT;

  return (ErrorCode) ret;
}

// find all bus participants and provide the address
ErrorCode I2C::Find_Bus_Partners(ByteBuffer &addresses)
{
  addresses.clear();

  if (! m_isInit) return E_I2C_NOT_INITIALIZED;

  char reg = 0;
  char buf = 0;

  for(int i = 0; i<7; i++) 
  {
    for(int j=0; j<0x10; j++) 
    {
      bcm2835_i2c_setSlaveAddress(i * 0x10 + j);        
      uint8_t code = bcm2835_i2c_read_register_rs(&reg, &buf, 1);
      if (code == 0) addresses.push_back(i*0x10+j);
    }
  }

  return E_OK;
}

// write n bytes to the given address
ErrorCode I2C::WriteByte(int address, uint8_t buf)
{
  ByteBuffer tmp;
  tmp.push_back(buf);
  return Write(address, tmp);
}

// read n bytes from the given address
ErrorCode I2C::ReadByte(int address, uint8_t &buf)
{
  buf = 0;

  ByteBuffer tmp;
  ErrorCode err = Read(address, tmp, 1);
  if (err == E_OK)
    buf = tmp[0];

  return err;
}

// write n bytes to the given register at address
ErrorCode I2C::WriteRegister(int address, int reg, const ByteBuffer &buf)
{
  ByteBuffer tmp;
  tmp.push_back((unsigned char)reg);
  tmp.insert(tmp.end(), buf.begin(), buf.end());

  return Write(address, tmp);
}
