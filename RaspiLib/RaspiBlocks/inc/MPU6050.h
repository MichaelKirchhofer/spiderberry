#pragma once

// MPU6050 Gyro Sensor 
// I2C controlled

#include <ErrorCode.h>
#include <Vector3D.h>

#include <string>

namespace PiLib
{
  const int MPU6050_REG_ACCEL_XOFFS_H = 0x06;
  const int MPU6050_REG_ACCEL_XOFFS_L = 0x07;
  const int MPU6050_REG_ACCEL_YOFFS_H = 0x08;
  const int MPU6050_REG_ACCEL_YOFFS_L = 0x09;
  const int MPU6050_REG_ACCEL_ZOFFS_H = 0x0A;
  const int MPU6050_REG_ACCEL_ZOFFS_L = 0x0B;
  const int MPU6050_REG_GYRO_XOFFS_H = 0x13;
  const int MPU6050_REG_GYRO_XOFFS_L = 0x14;
  const int MPU6050_REG_GYRO_YOFFS_H = 0x15;
  const int MPU6050_REG_GYRO_YOFFS_L = 0x16;
  const int MPU6050_REG_GYRO_ZOFFS_H = 0x17;
  const int MPU6050_REG_GYRO_ZOFFS_L = 0x18;
  const int MPU6050_REG_SMPLRT_DIV = 0x19;
  const int MPU6050_REG_CONFIG = 0x1A;
  const int MPU6050_REG_GYRO_CONFIG = 0x1B; // Gyroscope Configuration
  const int MPU6050_REG_ACCEL_CONFIG = 0x1C; // Accelerometer Configuration
  const int MPU6050_REG_FF_THRESHOLD = 0x1D;
  const int MPU6050_REG_FF_DURATION = 0x1E;
  const int MPU6050_REG_MOT_THRESHOLD = 0x1F;
  const int MPU6050_REG_MOT_DURATION = 0x20;
  const int MPU6050_REG_ZMOT_THRESHOLD = 0x21;
  const int MPU6050_REG_ZMOT_DURATION = 0x22;
  const int MPU6050_REG_INT_PIN_CFG = 0x37; // INT Pin. Bypass Enable Configuration
  const int MPU6050_REG_INT_ENABLE = 0x38; // INT Enable
  const int MPU6050_REG_INT_STATUS = 0x3A;
  const int MPU6050_REG_ACCEL_XOUT_H = 0x3B;
  const int MPU6050_REG_ACCEL_XOUT_L = 0x3C;
  const int MPU6050_REG_ACCEL_YOUT_H = 0x3D;
  const int MPU6050_REG_ACCEL_YOUT_L = 0x3E;
  const int MPU6050_REG_ACCEL_ZOUT_H = 0x3F;
  const int MPU6050_REG_ACCEL_ZOUT_L = 0x40;
  const int MPU6050_REG_TEMP_OUT_H = 0x41;
  const int MPU6050_REG_TEMP_OUT_L = 0x42;
  const int MPU6050_REG_GYRO_XOUT_H = 0x43;
  const int MPU6050_REG_GYRO_XOUT_L = 0x44;
  const int MPU6050_REG_GYRO_YOUT_H = 0x45;
  const int MPU6050_REG_GYRO_YOUT_L = 0x46;
  const int MPU6050_REG_GYRO_ZOUT_H = 0x47;
  const int MPU6050_REG_GYRO_ZOUT_L = 0x48;
  const int MPU6050_REG_MOT_DETECT_STATUS = 0x61;
  const int MPU6050_REG_MOT_DETECT_CTRL = 0x69;
  const int MPU6050_REG_USER_CTRL = 0x6A; // User Control
  const int MPU6050_REG_PWR_MGMT_1 = 0x6B; // Power Management 1
  const int MPU6050_REG_PWR_MGMT_2 = 0x6C; // Power Management 1
  const int MPU6050_REG_WHO_AM_I = 0x75; // Who Am I

  const float G = 9.80665f;   // the constant G [m/s2]

  enum MPU6050_ClockSource
  {
    MPU6050_CLOCK_INTERNAL_8MHZ   = 0,
    MPU6050_CLOCK_PLL_XGYRO       = 1,
    MPU6050_CLOCK_PLL_YGYRO       = 2,
    MPU6050_CLOCK_PLL_ZGYRO       = 3,
    MPU6050_CLOCK_EXTERNAL_32KHZ  = 4,
    MPU6050_CLOCK_EXTERNAL_19MHZ  = 5,
    MPU6050_CLOCK_KEEP_RESET      = 7,
  };

  enum MPU6050_Dps     // degree per second
  {
    MPU6050_SCALE_250DPS          = 0,
    MPU6050_SCALE_500DPS          = 1,
    MPU6050_SCALE_1000DPS         = 2,
    MPU6050_SCALE_2000DPS         = 3,
  };

  enum MPU6050_Range  // gyroscope full scale range
  {
    MPU6050_RANGE_2G              = 0,
    MPU6050_RANGE_4G              = 1,
    MPU6050_RANGE_8G              = 2,
    MPU6050_RANGE_16G             = 3,
  };

  enum MPU6050_OnDelay
  {
    MPU6050_NO_DELAY              = 0,
    MPU6050_DELAY_1MS             = 1,
    MPU6050_DELAY_2MS             = 2,
    MPU6050_DELAY_3MS             = 3,
  };

  enum MPU6050_Dhpf  // digital high pass filter
  {
    MPU6050_DHPF_RESET            = 0b000,
    MPU6050_DHPF_5HZ              = 0b001,
    MPU6050_DHPF_2_5HZ            = 0b010,
    MPU6050_DHPF_1_25HZ           = 0b011,
    MPU6050_DHPF_0_63HZ           = 0b100,
    MPU6050_DHPF_HOLD             = 0b111,
  };

  enum MPU6050_Dlpf  // digital low pass filter
  {
    MPU6050_DLPF_0                = 0b000,
    MPU6050_DLPF_1                = 0b001,
    MPU6050_DLPF_2                = 0b010,
    MPU6050_DLPF_3                = 0b011,
    MPU6050_DLPF_4                = 0b100,
    MPU6050_DLPF_5                = 0b101,
    MPU6050_DLPF_6                = 0b110,
  };

  class MPU6050
  {
    public:
      MPU6050(int address = 0x68);
      ~MPU6050();

      const Vector3D & RawAcceleration() const {return m_rawAcceleration;}
      const Vector3D & RawGyro() const {return m_rawGyroscope;}

      const Vector3D & Acceleration() const {return m_acceleration;}
      const Vector3D & Gyro() const {return m_gyroscope;}

      // --------- chip communication ---------------------
      // update data from chip
      ErrorCode UpdateGyroData();

      // low level access - debugging only
      ErrorCode Write8(int reg, int value);    // write 1 byte
      ErrorCode Read8(int reg, int &value);    // read one byte
      ErrorCode Write16(int reg, int value);   // write 2 bytes
      ErrorCode Read16(int reg, int &value);   // read 2 bytes

      // more settings - direct register access
      ErrorCode SetClockSource(MPU6050_ClockSource source);
      ErrorCode SetScale(MPU6050_Dps scale);
      ErrorCode SetRange(MPU6050_Range range);
      ErrorCode SetDHPFMode(MPU6050_Dhpf dhpf);
      ErrorCode SetDLPFMode(MPU6050_Dlpf dlpf);

	    ErrorCode GetClockSource(MPU6050_ClockSource &source);
      ErrorCode GetScale(MPU6050_Dps &scale);
      ErrorCode GetRange(MPU6050_Range &range);

      ErrorCode GetTemperature(float &value);

      // get current values as string
      std::string ToString();

    private:
      int m_address = 0;

      // setup MPU6050 for proper operation
      void Init();

      // current values, can be reported with ToString()
      Vector3D m_rawAcceleration;  // raw data
      Vector3D m_rawGyroscope;
      Vector3D m_acceleration;     // netto data
      Vector3D m_gyroscope;
      float m_temperature;         // only used internally

    	float m_dpsPerDigit;           // scaling factor
      float m_rangePerDigit;
  };
}
 