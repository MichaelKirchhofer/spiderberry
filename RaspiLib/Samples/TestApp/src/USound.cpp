#include "RTCTest.h"

#include <RaspiLib.h>
#include <UltraSound.h>

#include <iostream>

using namespace PiLib;
using namespace std;

//const RaspiPinLabel SpiderTriggerPin = PIN13_GPIO27_PCMD;
//const RaspiPinLabel SpiderEchoPin = PIN15_GPIO22;

const RaspiPinLabel SpiderTriggerPin = PIN11_GPIO17;
const RaspiPinLabel SpiderEchoPin = PIN12_GPIO18_PCMC;

void USoundDistance()
{
  UltraSound usound(SpiderTriggerPin, SpiderEchoPin);

  cout << "activating ultrasound ..." << endl;

  for (int i=0; i<10; i++)
  {
    double distance = usound.GetDistance();
    cout << "Distance in cm is " << distance << endl;

    Utils::delay_ms(1000);
  }

}
