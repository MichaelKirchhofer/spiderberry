#include "LPS25H.h"

#include <RaspiLib.h>

using namespace PiLib;

enum class LPS25H_Registers
{
  REF_P_XL     = 0x08,
  REF_P_L      = 0x09,
  REF_P_H      = 0x0A,
  WHO_AM_I     = 0x0F,
  RES_CONF     = 0x10,
  CTRL_REG1    = 0x20,
  CTRL_REG2    = 0x21,
  CTRL_REG3    = 0x22,
  CTRL_REG4    = 0x23,
  INT_CFG      = 0x24,
  INT_SOURCE   = 0x25,
  STATUS       = 0x27,
  PRESS_OUT_XL = 0x28,
  PRESS_OUT_L  = 0x29,
  PRESS_OUT_H  = 0x2A,
  TEMP_OUT_L   = 0x2B,
  TEMP_OUT_H   = 0x2C,
  FIFO_CTRL    = 0x2E,
  FIFO_STATUS  = 0x2F,
  THS_P_L      = 0x30,
  THS_P_H      = 0x31,
  RPDS_L       = 0x39,
  RPDS_H       = 0x40,
};

LPS25H::LPS25H(int address)
{
  m_address = address;
  init();
}

LPS25H::~LPS25H()
{ }

// init device
bool LPS25H::init()
{
  II2c &iface = RaspiLib::GetInstance().GetI2c();

  // set control register to Power On, 25 Hz scan, block data update -> 0xC4
  ByteBuffer bb;
  bb.emplace_back(0xC4);
  ErrorCode err = iface.WriteRegister(m_address, (uint8_t)LPS25H_Registers::CTRL_REG1, bb);

  bb.clear();
  // resolution settings default: 0x05
  bb.emplace_back(0x05);
  err = iface.WriteRegister(m_address, (uint8_t)LPS25H_Registers::RES_CONF, bb);

  bb.clear();
  // fifo control: FIFO_MEAN_MODE 0xC0
  bb.emplace_back(0xC0);
  err = iface.WriteRegister(m_address, (uint8_t)LPS25H_Registers::FIFO_CTRL, bb);

  // set control register to normal mode -> 0x40
  bb.emplace_back(0x40);
  err = iface.WriteRegister(m_address, (uint8_t)LPS25H_Registers::CTRL_REG2, bb);

  m_isInit = true;
  return true;
}

double LPS25H::getPressure()
{
  updateData();
  return (m_pValid) ? m_pressure : 0.0;
}

double LPS25H::getTemperature()
{
  updateData();
  return (m_tValid) ? m_temperature : -273,23;
}

bool LPS25H::updateData()
{
  m_pValid = m_tValid = false;
  if (! m_isInit)
    init();

  if (! m_isInit) return false;

  m_temperature = m_pressure = 0.;

  II2c &iface = RaspiLib::GetInstance().GetI2c();

  ByteBuffer bb;
  ErrorCode err = iface.ReadRegister_RepeatStart(m_address, (int)LPS25H_Registers::STATUS, bb, 1);

  uint8_t status = bb[0];

  if (status & 2)
  {
    bb.clear();
    err = iface.ReadRegister_RepeatStart(m_address, (int)LPS25H_Registers::PRESS_OUT_XL, bb, 3);
    int32_t value = (int32_t)bb[2] * 65536 + (int32_t)bb[1] * 256 + bb[0];
    m_pressure = (double)value / 4096.;
    m_pValid = true;
  }

  if (status & 1)
  {
    bb.clear();
    err = iface.ReadRegister_RepeatStart(m_address, (int)LPS25H_Registers::TEMP_OUT_L, bb, 2);
    int16_t value = (int16_t)bb[1] * 256 + bb[0];
    m_temperature = (double)value / 480. + 42.5;
    m_tValid = true;
  }

  return true;
}
