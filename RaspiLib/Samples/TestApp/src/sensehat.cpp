#include "sensehat.h"

#include <RaspiLib.h>
#include <HTS221.h>
#include <LPS25H.h>

#include <iostream>

using namespace PiLib;
using namespace std;

void doSenseHatTest()
{
  HTS221 humidityDevice;

  for (int i= 0; i<10; i++)
  {
    cout << "Humidity: " << humidityDevice.getHumidity() << ", Temperature: " << humidityDevice.getTemperature() << endl;
  }

  LPS25H pressureDevice;

  for (int i= 0; i<10; i++)
  {
    cout << "Pressure: " << pressureDevice.getPressure() << ", Temperature: " << pressureDevice.getTemperature() << endl;
  }

}
