#pragma once

// ultrasound module
// one pin trigger, one pin echo

// for spider, trigger is GPIO27 and echo is GPIO 22
// in RaspiLib, these are pins PIN13_GPIO27_PCMD and PIN15_GPIO22

#include <ErrorCode.h>
#include <IPin.h>

namespace PiLib
{
  class UltraSound
  {
    public:
      UltraSound(RaspiPinLabel triggerPin, RaspiPinLabel echoPin);
      ~UltraSound();

      double GetDistance();

    private:
      IPin *m_trigger = nullptr;
      IPin *m_echo = nullptr;

      void pulseTrigger();
      void waitForEcho(bool state, int timeout);
  };
}