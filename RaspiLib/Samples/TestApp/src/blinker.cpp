#include "blinker.h"

#include <RaspiLib.h>

using namespace PiLib;

void Blinker()
{
  IPin &pin = RaspiLib::GetInstance().GetPin(PIN37_GPIO26);
  if (pin.GetGPIONumber() != GPIO_DUMMY)
  {
    pin.SetPinMode(FSEL_OUTP);
    
    for (int i=0; i<10; i++)
    {
      pin.WriteBool(true);
      Utils::delay_ms(500);
      pin.WriteBool(false);
      Utils::delay_ms(500);
    }
  }
}

