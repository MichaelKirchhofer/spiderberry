#include "MPU6050.h"

#include <RaspiLib.h>

using namespace PiLib;

MPU6050::MPU6050(int address) : m_address(address)
{ 
  Init(); 
}

MPU6050::~MPU6050()
{ }

void MPU6050::Init()
{
  SetClockSource(MPU6050_CLOCK_PLL_XGYRO);
  SetScale(MPU6050_SCALE_2000DPS);
  SetRange(MPU6050_RANGE_2G);

  // set sample rate to 1/8
  Write8(MPU6050_REG_SMPLRT_DIV, 7);
  // disable sleep mode
  Write8(MPU6050_REG_PWR_MGMT_1, MPU6050_CLOCK_PLL_XGYRO);
}

// update data from chip
ErrorCode MPU6050::UpdateGyroData()
{
  // read acclelertion ( 3x2 bytes )
  // read temp (2 bytes)
  // read gyro ( 3x2 bytes)

  II2c &iface = RaspiLib::GetInstance().GetI2c();
  ByteBuffer bb;

 ErrorCode err = iface.ReadRegister_RepeatStart(m_address,MPU6050_REG_ACCEL_XOUT_H, bb, 14);  // read 14 bytes
  if (err == E_OK)
  {
    // acceleration
    int valx = bb[0] * 256 + bb[1];
    int valy = bb[2] * 256 + bb[3];
    int valz = bb[4] * 256 + bb[5];

    // check for negative values 
    if (valx > 32767) valx -= 65536;
    if (valy > 32767) valy -= 65536;
    if (valz > 32767) valz -= 65536;

    m_rawAcceleration = Vector3D((float)valx, (float)valy, (float)valz);
    m_acceleration = m_rawAcceleration * m_rangePerDigit; 
    int t= bb[6] * 256 + bb[7];
    if (t>32767) t-= 65536;
    m_temperature = ((float) t / 340. + 36.53);

    // gyroscope
    valx = bb[ 8] * 256 + bb[ 9];
    valy = bb[10] * 256 + bb[11];
    valz = bb[12] * 256 + bb[13];

    // check for negative values 
    if (valx > 32767) valx -= 65536;
    if (valy > 32767) valy -= 65536;
    if (valz > 32767) valz -= 65536;

    m_rawGyroscope = Vector3D((float)valx, (float)valy, (float)valz);
    m_gyroscope = m_rawGyroscope * m_dpsPerDigit;
  }

  return err;
}

// low level access - debugging only
ErrorCode MPU6050::Write8(int reg, int value)
{
  II2c &iface = RaspiLib::GetInstance().GetI2c();
  ByteBuffer bb;
  bb.emplace_back(value & 0xFF);  // high byte
  ErrorCode err = iface.WriteRegister(m_address, reg, bb);
  return err;
}

ErrorCode MPU6050::Read8(int reg, int &value)
{
  value = 0;

  II2c &iface = RaspiLib::GetInstance().GetI2c();
  ByteBuffer bb;

  ErrorCode err = iface.ReadRegister_RepeatStart(m_address, reg & 0xFF, bb, 1);  // read 1 byte
  if (err == E_OK)
    value = bb[0];

  return err;
}

ErrorCode MPU6050::Write16(int reg, int value)
{
  II2c &iface = RaspiLib::GetInstance().GetI2c();
  ByteBuffer bb;
  bb.emplace_back((value >> 8) & 0xFF);  // high byte
  bb.emplace_back(value & 0xFF);         // low byte

  ErrorCode err = iface.WriteRegister(m_address, reg, bb);
  return err;
}

ErrorCode MPU6050::Read16(int reg, int &value)
{
  value = 0;

  II2c &iface = RaspiLib::GetInstance().GetI2c();
  ByteBuffer bb;

  ErrorCode err = iface.ReadRegister_RepeatStart(m_address, reg & 0xFF, bb, 2);  // read 2 bytes
  if (err == E_OK)
  {
    value = bb[0] * 256 + bb[1];
  }

  return err;
}

ErrorCode MPU6050::SetClockSource(MPU6050_ClockSource source)
{
  int value = 0;
  ErrorCode err = Read8(MPU6050_REG_PWR_MGMT_1, value);
  if (err == E_OK)
  {
    value &= 0xF8; // mask out bit 0 .. 2
    value |= source;

    err = Write8(MPU6050_REG_PWR_MGMT_1, value);
  }
  return err;
}

ErrorCode MPU6050::SetScale(MPU6050_Dps scale)
{
  int value = 0;

  switch (scale)
  {
    case MPU6050_SCALE_250DPS:
      m_dpsPerDigit = .007633f;
      break;
    case MPU6050_SCALE_500DPS:
      m_dpsPerDigit = .015267f;
      break;
    case MPU6050_SCALE_1000DPS:
      m_dpsPerDigit = .030487f;
      break;
    case MPU6050_SCALE_2000DPS:
      m_dpsPerDigit = .060975f;
      break;
    default:
      break;
  }

  ErrorCode err = Read8(MPU6050_REG_GYRO_CONFIG, value);
  if (err == E_OK)
  {
    value &= 0xE7; // mask out bit 4 .. 3
    value |= (scale << 3);

    err = Write8(MPU6050_REG_GYRO_CONFIG, value);
  }
  return err;
}

ErrorCode MPU6050::SetRange(MPU6050_Range range)
{
  int value = 0;

  switch (range)
  {
    case MPU6050_RANGE_2G:
      m_rangePerDigit = .000061f;
      break;
    case MPU6050_RANGE_4G:
      m_rangePerDigit = .000122f;
      break;
    case MPU6050_RANGE_8G:
      m_rangePerDigit = .000244f;
      break;
    case MPU6050_RANGE_16G:
      m_rangePerDigit = .0004882f;
      break;
    default:
      break;
  }

  ErrorCode err = Read8(MPU6050_REG_ACCEL_CONFIG, value);
  if (err == E_OK)
  {
    value &= 0xE7; // mask out bit 4 .. 3
    value |= (range << 3);

    err = Write8(MPU6050_REG_ACCEL_CONFIG, value);
  }

  return err;
}

ErrorCode MPU6050::SetDHPFMode(MPU6050_Dhpf dhpf)
{
  int value = 0;

  ErrorCode err = Read8(MPU6050_REG_ACCEL_CONFIG, value);
  if (err == E_OK)
  {
    value &= 0xF8; // mask out bit 4 .. 3
    value |= dhpf;

    err = Write8(MPU6050_REG_ACCEL_CONFIG, value);
  }

  return err;
}

ErrorCode MPU6050::SetDLPFMode(MPU6050_Dlpf dlpf)
{
  int value = 0;

  ErrorCode err = Read8(MPU6050_REG_CONFIG, value);
  if (err == E_OK)
  {
    value &= 0xF8; // mask out bit 4 .. 3
    value |= dlpf;

    err = Write8(MPU6050_REG_CONFIG, value);
  }

  return err;
}

ErrorCode MPU6050::GetClockSource(MPU6050_ClockSource &source)
{
  int value = 0;
  ErrorCode err = Read8(MPU6050_REG_PWR_MGMT_1, value);
  if (err == E_OK)
  {
    source = (MPU6050_ClockSource) (value & 0x07);
  }

  return err;
}

ErrorCode MPU6050::GetScale(MPU6050_Dps &scale)
{
  int value = 0;

  ErrorCode err = Read8(MPU6050_REG_GYRO_CONFIG, value);
  if (err == E_OK)
  {
    value &= 0x18; // mask out bit 4 .. 3
    scale = (MPU6050_Dps) (value >> 3);
  }

  return err;
}

ErrorCode MPU6050::GetRange(MPU6050_Range &range)
{
  int value = 0;

  ErrorCode err = Read8(MPU6050_REG_ACCEL_CONFIG, value);
  if (err == E_OK)
  {
    value &= 0x18; // mask out bit 4 .. 3
    range = (MPU6050_Range) (value >> 3);
  }

  return err;
}

ErrorCode MPU6050::GetTemperature(float &value)
{
  int ival = 0;
  value = 0.;
  ErrorCode err = Read16(MPU6050_REG_TEMP_OUT_H, ival);
  if (err == E_OK)
  {
    value = (float) (ival/340. + 36.53);
  }

  return err;
}

std::string MPU6050::ToString()
{
  std::string ret = "MPU6050 data: Temperature " + std::to_string(m_temperature) + "\r\n";
  ret += "Acceleration: " + m_acceleration.ToString() + " (" + m_rawAcceleration.ToString() + ")\r\n";
  ret += "Gyroscope   : " + m_gyroscope.ToString() + " (" + m_rawGyroscope.ToString() + ")";

  return ret;
}
