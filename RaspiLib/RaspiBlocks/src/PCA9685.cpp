#include "PCA9685.h"

#include <RaspiLib.h>

using namespace PiLib;

PCA9685::PCA9685(uint8_t address) : m_address(address)
{ }

PCA9685::~PCA9685()
{ }

ErrorCode PCA9685::Init()
{
  ErrorCode err = SetPWMFrequency(1000);
  return err;
}

ErrorCode PCA9685::Reset()
{
  ErrorCode err = Write8(PCA9685_REG_MODE1, PCA9685_MODE1_RESTART);
  Utils::delay_ms(100);

  return err;
}

ErrorCode PCA9685::SetPWMFrequency(uint16_t value)
{
  if (value < 1) value = 1;
  if (value > 3500) value = 3500;

  double val = ((m_frequency / (value * 4096.0)) + 0.5) - 1;
  if (val < PCA9685_PRESCALE_MIN) val = PCA9685_PRESCALE_MIN;
  if (val > PCA9685_PRESCALE_MAX) val = PCA9685_PRESCALE_MAX;
  uint8_t prescale = (uint8_t)val;

  int currentMode = 0;
  Read8(PCA9685_REG_MODE1, currentMode);
  int newmode = (currentMode & ~PCA9685_MODE1_RESTART) | PCA9685_MODE1_SLEEP;
  Write8(PCA9685_REG_MODE1, newmode);  // goto sleep
  // chane prescaler
  Write8(PCA9685_REG_PRESCALE, prescale);
  Utils::delay_ms(10);

  currentMode = (currentMode & ~PCA9685_MODE1_SLEEP) | PCA9685_MODE1_RESTART | PCA9685_MODE1_AI;
  // clear the SLEEP bit to start
  Write8(PCA9685_REG_MODE1, currentMode);

  return E_OK;
}

ErrorCode PCA9685::GetPrescale(uint8_t &prescale)
{
  prescale = 0;

  int value = 0;
  ErrorCode err = Read8(PCA9685_REG_PRESCALE, value);
  if (err == E_OK)
    prescale = value;

  return err;
}

ErrorCode PCA9685::GetPWM(uint8_t channel, uint16_t &on, uint16_t &off)
{
  if (channel > 15) return E_INVALID_PARAM;

  on = off = 0;
  int registerOffset = PCA9685_REG_CHANNEL0_ON_L + channel * 4;
  
  II2c &iface = RaspiLib::GetInstance().GetI2c();
  ByteBuffer bb;

  ErrorCode err = iface.ReadRegister_RepeatStart(m_address, registerOffset & 0xFF, bb, 4);  // read 4 bytes
  if (err == E_OK)
  {
    on = bb[0] + bb[1] * 256;
    off = bb[2] + bb[3] * 256;
  }

  return err;
}

ErrorCode PCA9685::SetPWM(uint8_t channel, uint16_t on, uint16_t off)
{
  if (channel > 15) return E_INVALID_PARAM;

  int registerOffset = PCA9685_REG_CHANNEL0_ON_L + channel * 4;
  // check auto off
  if (off == 0 && on != 0) off = 4096 - on; 
  
  II2c &iface = RaspiLib::GetInstance().GetI2c();
  ByteBuffer bb;
  //bb.emplace_back(registerOffset & 0xFF);    
  bb.emplace_back(on & 0xFF);
  bb.emplace_back( (on / 256) & 0xFF );
  bb.emplace_back(off & 0xFF);
  bb.emplace_back( (off / 256) & 0xFF );

  ErrorCode err = iface.WriteRegister(m_address, registerOffset & 0xFF, bb);

  return err;
}

// set servo angle 0 .. 180 degrees
ErrorCode PCA9685::SetServoAngle(uint8_t channel, float angle_degrees)
{
  if (channel > 15) return E_INVALID_PARAM;
  if (angle_degrees < 0. || angle_degrees > 180.) return E_INVALID_PARAM;

  // transform angle 0..180 into usec pulse 500..2500
  float val = LinearTransform(angle_degrees, 0., 180., 500., 2500.);
  // transform usec (0..20000, 20ms servo interval) into on/off (0..4095) 
  float duty = LinearTransform(val, 0., 20000., 0., 4095.);

  ErrorCode err = SetPWM(channel, 0, (uint16_t) duty);
  return err;
}

ErrorCode PCA9685::SetPinMode(PCA9685_Mode2 val)
{
  ErrorCode err = Write8(PCA9685_REG_MODE2, val);
  Utils::delay_ms(100);

  return err;
}

// low level access - debugging only
ErrorCode PCA9685::Write8(int reg, int value)
{
  II2c &iface = RaspiLib::GetInstance().GetI2c();
  ByteBuffer bb;
  // bb.emplace_back(reg & 0xFF);    
  bb.emplace_back(value & 0xFF);

  ErrorCode err = iface.WriteRegister(m_address, reg & 0xFF, bb);
  return err;
}

ErrorCode PCA9685::Read8(int reg, int &value)
{
  value = 0;

  II2c &iface = RaspiLib::GetInstance().GetI2c();
  ByteBuffer bb;

  ErrorCode err = iface.ReadRegister_RepeatStart(m_address, reg & 0xFF, bb, 1);  // read 1 byte
  if (err == E_OK)
    value = bb[0];

  return err;
}

float PCA9685::LinearTransform(float value, float fromLow, float fromHigh, float toLow, float toHigh)
{
  float res = 0.;

  res = (toHigh - toLow) * (value - fromLow) / (fromHigh - fromLow) + toLow;
  return res;
}
