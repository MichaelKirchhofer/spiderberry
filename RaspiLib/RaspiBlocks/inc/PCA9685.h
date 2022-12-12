#pragma once

// PCA9685 16 channel PWM LED/Servo Driver
// I2C controlled
// default address 0x40

#include <ErrorCode.h>
#include <stdint.h>

namespace PiLib
{
  const int PCA9685_REG_MODE1 = 0x00;          // Mode Register 1
  const int PCA9685_REG_MODE2 = 0x01;          // Mode Register 2 
  const int PCA9685_REG_SUBADR1 = 0x02;        // I2C-bus subaddress 1 
  const int PCA9685_REG_SUBADR2 = 0x03;        // I2C-bus subaddress 2 
  const int PCA9685_REG_SUBADR3 = 0x04;        // I2C-bus subaddress 3 
  const int PCA9685_REG_ALLCALLADR = 0x05;     // All Call I2C-bus address 
  // starting channel registers 4 per channel
  const int PCA9685_REG_CHANNEL0_ON_L = 0x06;  // CHANNEL0 on tick, low byte
  const int PCA9685_REG_CHANNEL0_ON_H = 0x07;  // CHANNEL0 on tick, high byte
  const int PCA9685_REG_CHANNEL0_OFF_L = 0x08; // CHANNEL0 off tick, low byte 
  const int PCA9685_REG_CHANNEL0_OFF_H = 0x09; // CHANNEL0 off tick, high byte 
  // etc all 16:  PCA9685_REG_CHANNEL15_OFF_H 0x45

  // general purpose
  const int PCA9685_REG_ALL_ON_L = 0xFA;       // load all the CHANNELn_ON registers, low 
  const int PCA9685_REG_ALL_ON_H = 0xFB;       // load all the CHANNELn_ON registers, high 
  const int PCA9685_REG_ALL_OFF_L = 0xFC;      // load all the CHANNELn_OFF registers, low 
  const int PCA9685_REG_ALL_OFF_H = 0xFD;      // load all the CHANNELn_OFF registers,high 
  const int PCA9685_REG_PRESCALE = 0xFE;       // Prescaler for PWM output frequency 
  const int PCA9685_REG_TESTMODE = 0xFF;       // defines the test mode to be entered 

  // some constants
  const int PCA9685_FREQUENCY_OSCILLATOR = 25000000; // Int. osc. frequency in datasheet
  const int PCA9685_PRESCALE_MIN = 3;          // minimum prescale value
  const int PCA9685_PRESCALE_MAX = 255;        // maximum prescale value

  // MODE1 bits
  enum PCA6985_Mode1
  {
    PCA9685_MODE1_ALLCAL = 0x01,  // respond to All Call I2C-bus address 
    PCA9685_MODE1_SUB3 = 0x02,    // respond to I2C-bus subaddress 3 
    PCA9685_MODE1_SUB2 = 0x04,    // respond to I2C-bus subaddress 2 
    PCA9685_MODE1_SUB1 = 0x08,    // respond to I2C-bus subaddress 1 
    PCA9685_MODE1_SLEEP = 0x10,   // Low power mode. Oscillator off 
    PCA9685_MODE1_AI = 0x20,      // Auto-Increment enabled 
    PCA9685_MODE1_EXTCLK = 0x40,  // Use EXTCLK pin clock 
    PCA9685_MODE1_RESTART = 0x80, // Restart enabled 
  };

  // MODE2 bits
  enum PCA9685_Mode2
  {
    PCA9685_MODE2_OUTNE_0 = 0x01, // Active LOW output enable input
    PCA9685_MODE2_OUTNE_1 = 0x02, // Active LOW output enable input - high impedience
    PCA9685_MODE2_OUTDRV = 0x04,  // totem pole structure vs open-drain
    PCA9685_MODE2_OCH = 0x08,     // Outputs change on ACK vs STOP
    PCA9685_MODE2_INVRT = 0x10,   // Output logic state inverted
  };

  class PCA9685
  {
  public:
    PCA9685(uint8_t address);
    ~PCA9685();

    ErrorCode Init();
    ErrorCode Reset();

    // frequ -> prescale value for the chip
    // 1 .. 3500 (datasheet 50 MHz / (4*4096))
    // for Servo init with 50 Hz
    ErrorCode SetPWMFrequency(uint16_t value);
    ErrorCode GetPrescale(uint8_t &prescale);

    // get the current settings for channel
    ErrorCode GetPWM(uint8_t channel, uint16_t &on, uint16_t &off);
    // set the modulation width, if on < 4096 the off value is calculated automatically
    ErrorCode SetPWM(uint8_t channel, uint16_t on, uint16_t off = 0);

    // set servo angle 0 .. 180 degrees
    ErrorCode SetServoAngle(uint8_t channel, float angle_degrees);

    ErrorCode SetPinMode(PCA9685_Mode2 val);

    // low level access - debugging only
    ErrorCode Write8(int reg, int value);    // write 1 byte
    ErrorCode Read8(int reg, int &value);    // read one byte

  private:
    uint8_t m_address = 0;
    uint32_t m_frequency = PCA9685_FREQUENCY_OSCILLATOR;

    // calculate fraction of value in from interval and transform it into to interval
    float LinearTransform(float value, float fromLow, float fromHigh, float toLow, float toHigh);
  };

}