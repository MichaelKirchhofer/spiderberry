#include "UltraSound.h"

#include <RaspiLib.h>

using namespace PiLib;

UltraSound::UltraSound(RaspiPinLabel triggerPin, RaspiPinLabel echoPin)
{
  // get pins & set reference
  IPin &pin = RaspiLib::GetInstance().GetPin(triggerPin);
  if (pin.GetGPIONumber() != GPIO_DUMMY)
    m_trigger = &pin;

  IPin &pin2 = RaspiLib::GetInstance().GetPin(echoPin);
  if (pin2.GetGPIONumber() != GPIO_DUMMY)
    m_echo = &pin2;

  pin.SetPinMode(FSEL_OUTP);
  pin2.SetPinMode(FSEL_INPT);
}

UltraSound::~UltraSound()
{
  m_echo = m_trigger = nullptr;
}

double UltraSound::GetDistance()
{
  // trigger = out
  // echo = in

  if (!m_trigger || ! m_echo) return 0.0;

  // make sure we are in right mode
  m_trigger->SetPinMode(FSEL_OUTP);
  m_echo->SetPinMode(FSEL_INPT);

  pulseTrigger();

  waitForEcho(true, 100000);
  uint64_t start = Utils::getExactTime_us();

  waitForEcho(false, 100000);
  uint64_t end = Utils::getExactTime_us();

  double dist = (end - start) / 1000000. * 34000 / 2;

  return dist;
}

void UltraSound::pulseTrigger()
{
  m_trigger->WriteBool(false);
  Utils::delay_us(5);
  m_trigger->WriteBool(true);
  Utils::delay_us(5);
  m_trigger->WriteBool(false);
}

void UltraSound::waitForEcho(bool state, int timeout)
{
  while (m_echo->ReadBool() != state && timeout --);
}

