#include "IPin.h"

#include "bcm2835.h"

namespace PiLib
{
  class Pin : public IPin 
  {
    public:

      Pin(RaspiPinLabel whichOne);
      virtual ~Pin();

      static bool IsValidPin(RaspiPinLabel whichOne) {return (findGPIOForPin(whichOne) != GPIO_DUMMY);}
      
      // interface functions
      RaspiPinLabel GetRaspiPinNumber() { return m_pin;}
      int GetGPIONumber() { return m_GPIONumber; }
      PinPullUpDownSelect GetPullUpDownConfig() { return m_pud; }
      PinFunctionSelect GetPinMode() { return m_mode; }

      virtual void SetPinMode(PinFunctionSelect mode);
      virtual void SetPullUpDnConfig(PinPullUpDownSelect pud);

      virtual bool ReadBool();
      virtual void WriteBool(bool value);

    private:
      RaspiPinLabel m_pin = PIN_UNDEFINED;
      int m_GPIONumber = GPIO_DUMMY;
      PinFunctionSelect m_mode = FSEL_INPT;
      PinPullUpDownSelect m_pud = GPUD_NONE;

      static int findGPIOForPin(RaspiPinLabel whichOne);  // translate RaspiPin to GPIO

      void readSettings();    // init pin with current register settings
  };

}