#include "HTS221.h"

#include <RaspiLib.h>

using namespace PiLib;

enum class HTS221_Registers
{
  WHO_AM_I   = 0x0F,
  AV_CONF    = 0x10,
  CTRL1      = 0x20,
  CTRL2      = 0x21,
  CTRL3      = 0x22,
  STATUS     = 0x27,
  HUMIDITY_L = 0x28,
  HUMIDITY_H = 0x29,
  TEMP_L     = 0x2A,
  TEMP_H     = 0x2B,
  // calibration registers
  H0_rH_x2   = 0x30,
  H1_rH_x2   = 0x31,
  T0_degC_x8 = 0x32,
  T1_degC_x8 = 0x33,
  T1_T0_msb  = 0x35,
  H0_T0_OUT  = 0x36,
  H1_T0_OUT  = 0x3A,
  T0_OUT     = 0x3C,
  T1_OUT     = 0x3E,
};

HTS221::HTS221(int address)
{
  m_address = address;
  init();
}

HTS221::~HTS221()
{ }

// read calibration settings
bool HTS221::init()
{
  double T0, T1, H0, H1;
  int16_t T0_out, T1_out, H0_T0_out, H1_T0_out;

  II2c &iface = RaspiLib::GetInstance().GetI2c();

  // set control register to Power On, full read, max speed -> 0x87
  ByteBuffer bb;
  bb.emplace_back(0x87);
  ErrorCode err = iface.WriteRegister(m_address, (uint8_t)HTS221_Registers::CTRL1, bb);

  // set AV_CONF samples to default: 0x1B (16 avg temp, 32 avg hum)
  bb.clear();
  bb.emplace_back(0x1B);
  err = iface.WriteRegister(m_address, (uint8_t)HTS221_Registers::AV_CONF, bb);

  // read calibration data

  // temperature, start with high byte
  bb.clear();
  err = iface.ReadRegister_RepeatStart(m_address, (int)HTS221_Registers::T1_T0_msb, bb, 1);
  uint16_t msb = (uint16_t)bb[0];

  bb.clear();
  err = iface.ReadRegister_RepeatStart(m_address, (int)HTS221_Registers::T0_degC_x8, bb, 1);
  uint16_t value = ((msb & 0x03) << 8) + bb[0];
  T0 = (double)value / 8.;

  bb.clear();
  err = iface.ReadRegister_RepeatStart(m_address, (int)HTS221_Registers::T1_degC_x8, bb, 1);
  value = ((msb & 0x0C) << 6) + bb[0];
  T1 = (double)value / 8.;

  bb.clear();
  err = iface.ReadRegister_RepeatStart(m_address, (int)HTS221_Registers::T0_OUT, bb, 2);
  T0_out = (int16_t)bb[1] * 256 + bb[0];

  bb.clear();
  err = iface.ReadRegister_RepeatStart(m_address, (int)HTS221_Registers::T1_OUT, bb, 2);
  T1_out = (int16_t)bb[1] * 256 + bb[0];

  // humidity
  bb.clear();
  err = iface.ReadRegister_RepeatStart(m_address, (int)HTS221_Registers::H0_rH_x2, bb, 1);
  H0 = (double)bb[0] / 2.;

  bb.clear();
  err = iface.ReadRegister_RepeatStart(m_address, (int)HTS221_Registers::H1_rH_x2, bb, 1);
  H1 = (double)bb[0] / 2.;

  bb.clear();
  err = iface.ReadRegister_RepeatStart(m_address, (int)HTS221_Registers::H0_T0_OUT, bb, 2);
  H0_T0_out = (int16_t)bb[1] * 256 + bb[0];

  bb.clear();
  err = iface.ReadRegister_RepeatStart(m_address, (int)HTS221_Registers::H1_T0_OUT, bb, 2);
  H1_T0_out = (int16_t)bb[1] * 256 + bb[0];

  // calc coefficients
  m_temp_k = (T1-T0)/(T1_out-T0_out);
  m_temp_d = T0 - m_temp_k*T0_out;

  m_hum_k = (H1-H0)/(H1_T0_out-H0_T0_out);
  m_hum_d = H0 - m_hum_k*H0_T0_out;

  m_isInit = true;
  return true;
}

double HTS221::getHumidity()
{
  updateData();
  return (m_humValid) ? m_humidity : 0.0;
}

double HTS221::getTemperature()
{
  updateData();
  return (m_tempValid) ? m_temp : -273,23;
}

bool HTS221::updateData()
{
  m_humValid = m_tempValid = false;
  if (! m_isInit)
    init();

  if (! m_isInit) return false;

  m_temp = m_humidity = 0.;

  II2c &iface = RaspiLib::GetInstance().GetI2c();

  ByteBuffer bb;
  ErrorCode err = iface.ReadRegister_RepeatStart(m_address, (int)HTS221_Registers::STATUS, bb, 1);

  uint8_t status = bb[0];

  // check for data
  if (status & 2)
  {
    bb.clear();
    err = iface.ReadRegister_RepeatStart(m_address, (int)HTS221_Registers::HUMIDITY_L, bb, 2);
    int16_t value = (int16_t)bb[1] * 256 + bb[0];
    m_humidity = (double)value*m_hum_k + m_hum_d;
    m_humValid = true;
  }

  if (status & 1)
  {
    bb.clear();
    err = iface.ReadRegister_RepeatStart(m_address, (int)HTS221_Registers::TEMP_L, bb, 2);
    int16_t value = (int16_t)bb[1] * 256 + bb[0];
    m_temp = (double)value*m_temp_k + m_temp_d;
    m_tempValid = true;
  }

  return true;
}
