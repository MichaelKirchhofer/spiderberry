#include "GyroTest.h"

#include <RaspiLib.h>
#include <MPU6050.h>

#include <iostream>

using namespace PiLib;
using namespace std;

void doGyroTest()
{
  MPU6050 myMpu;

  for (auto i=0; i<60; i++)
  {
    myMpu.UpdateGyroData();
    cout << myMpu.ToString() << endl;

    Utils::delay_ms(100);
  }

}