#include "I2C.h"

#include "smbus.h"

using namespace PiLib;

I2C::I2C()
{}

I2C::~I2C()
{}

ErrorCode I2C::init()
{
  // int res = bcm2835_i2c_begin();
  // m_isInit = (res != 0);

  // if (m_isInit)
  //   // run at 100 kHz
  //   bcm2835_i2c_setClockDivider(BCM2835_I2C_CLOCK_DIVIDER_2500);

 
  // on the Pi we use I2C bus #1
  m_file = init_smbus(1);
  m_isInit = (m_file >= 0);

  return (m_isInit) ? E_OK : E_I2C_NOT_INITIALIZED;
}

ErrorCode I2C::shutdown()
{
  close_smbus(m_file);

  return E_OK;
}

// write n bytes to the given address
ErrorCode I2C::WriteByte(int address, uint8_t buf)
{
  if (! m_isInit) return E_I2C_NOT_INITIALIZED;
 
  setSlaveAddress(m_file, address);

  int res = i2c_smbus_write_byte(m_file, buf);
  if (res != 0)
    return E_I2C_ERROR_DATA;

  return E_OK;
}

// read n bytes from the given address
ErrorCode I2C::ReadByte(int address, uint8_t &buf)
{
  buf = 0;

  if (! m_isInit) return E_I2C_NOT_INITIALIZED;

  setSlaveAddress(m_file, address);

  int res = i2c_smbus_read_byte(m_file);
  if (res < 0)
    return E_I2C_ERROR_DATA;

  buf = res & 0xFF;
  return E_OK; 
}

// write n bytes to the given register at address
ErrorCode I2C::WriteRegister(int address, int reg, const ByteBuffer &buf)
{
  if (! m_isInit) return E_I2C_NOT_INITIALIZED;

  int length = buf.size();

  setSlaveAddress(m_file, address);

  int res = i2c_smbus_write_i2c_block_data(m_file, reg, length, (const uint8_t *)buf.data());
  if (res != 0)
    return E_I2C_ERROR_DATA;

  return E_OK;
}


// read register content from address, repeat start signal without stop
ErrorCode I2C::ReadRegister_RepeatStart(int address, int reg, ByteBuffer &buf, int len)
{
  buf.clear();

  if (! m_isInit) return E_I2C_NOT_INITIALIZED;
  if (! len) return E_INVALID_PARAM;

  setSlaveAddress(m_file, address);

  buf.reserve(len);

  int res = i2c_smbus_read_i2c_block_data(m_file, reg, len, (uint8_t *)buf.data());
  if (res == 0)   // should be some data in here!
    return E_I2C_ERROR_DATA;

  return E_OK;
}

// write buffer and read answer with repeated start in between
ErrorCode I2C::Write_Read_RepeatStart(int address, const ByteBuffer &sbuf, ByteBuffer &rbuf, int rlen)
{
  rbuf.clear();

  if (! m_isInit) return E_I2C_NOT_INITIALIZED;

  ErrorCode ret = E_OK;

  // bcm2835_i2c_setSlaveAddress(address);
  
  // rbuf.reserve(rlen);

  // uint8_t ret = bcm2835_i2c_write_read_rs((char *)sbuf.data(), sbuf.size(), (char *)rbuf.data(), rlen);

  // if (ret & BCM2835_I2C_REASON_ERROR_DATA) ret = E_I2C_ERROR_DATA;
  // else if (ret & BCM2835_I2C_REASON_ERROR_NACK) ret = E_I2C_ERROR_NACK;
  // else if (ret & BCM2835_I2C_REASON_ERROR_CLKT) ret = E_I2C_ERROR_CLKT;

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
      setSlaveAddress(m_file, i * 0x10 + j);
      int32_t code = i2c_smbus_read_byte_data(m_file, 0);

      if (code >= 0) addresses.push_back(i*0x10+j);
    }
  }

  return E_OK;
}
