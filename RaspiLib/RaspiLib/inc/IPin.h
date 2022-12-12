#pragma once

namespace PiLib
{

  // 40 pins on the extension board
  enum RaspiPinLabel
  {
    PIN_UNDEFINED = 0,
    PIN1_3v3 = 1,
    PIN2_5v,
    PIN3_GPIO2_SDA,         // I2C
    PIN4_5v,
    PIN5_GPIO3_SCL,         // I2C
    PIN6_GND,
    PIN7_GPIO4_GPCLK0,      // one wire protocol
    PIN8_GPIO14_TXD,        // UART
    PIN9_GND,
    PIN10_GPIO15_RXD,       // UART
    PIN11_GPIO17,
    PIN12_GPIO18_PCMC,
    PIN13_GPIO27_PCMD,
    PIN14_GND,
    PIN15_GPIO22,
    PIN16_GPIO23,
    PIN17_3v3,
    PIN18_GPIO24,
    PIN19_GPIO10_MOSI,
    PIN20_GND,
    PIN21_GPIO9_MISO,
    PIN22_GPIO25,
    PIN23_GPIO11_SCKL,
    PIN24_GPIO8_CE0,
    PIN25_GND,
    PIN26_GPIO7_CE1,
    PIN27_GPIO0_IDSD,
    PIN28_GPIO1_IDSC,
    PIN29_GPIO5,
    PIN30_GND,
    PIN31_GPIO6,
    PIN32_GPIO12,
    PIN33_GPIO13,
    PIN34_GND,
    PIN35_GPIO19_MISO,
    PIN36_GPIO16,
    PIN37_GPIO26,
    PIN38_GPIO20_MOSI,
    PIN39_GND,
    PIN40_GPIO21_SCLK,
    PIN_SENTINEL,          // last one
  };

  enum PinFunctionSelect
  {
    FSEL_INPT =	0b000,
    FSEL_OUTP	= 0b001,
    FSEL_ALT0 = 0b100,
    FSEL_ALT1 = 0b101,
    FSEL_ALT2 = 0b110,
    FSEL_ALT3 = 0b111,
    FSEL_ALT4 = 0b011, 
    FSEL_ALT5 = 0b010,
  };

  enum PinPullUpDownSelect
  {
    GPUD_NONE = 0,
    GPUD_DN = 1,
    GPUD_UP = 2,
  };

  // this is the fallback if the pin is not a "real" GPIO pin
  const int GPIO_DUMMY = -1;

  class IPin
  {
    public:
      virtual RaspiPinLabel GetRaspiPinNumber() = 0;
      virtual int GetGPIONumber() = 0;
      virtual PinPullUpDownSelect GetPullUpDownConfig() = 0;
      virtual PinFunctionSelect GetPinMode() = 0;

      virtual void SetPinMode(PinFunctionSelect mode) = 0;
      virtual void SetPullUpDnConfig(PinPullUpDownSelect pud) = 0;

      virtual bool ReadBool() = 0;
      virtual void WriteBool(bool value) = 0;
  };

};