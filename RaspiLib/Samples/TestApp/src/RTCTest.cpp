#include "RTCTest.h"

#include <RaspiLib.h>
#include <PCF8563.h>

#include <iostream>

using namespace PiLib;
using namespace std;

void testRTC()
{
  // prepare time info
  TimeInfo ti;
  TimeInfo ti_back;

  ti.year = 21;
  ti.month = 12;
  ti.day = 8;
  ti.weekday = 3;
  ti.hour = 14;
  ti.minute = 10;
  ti.second = 42;

  PCF8563 myRTC(0x51);

  cout << "setting time " << myRTC.ToString(ti) << endl;
  ErrorCode err = myRTC.SetTime(ti);
  cout << "waiting 10 secs ... " << endl;

  Utils::delay_ms(10000);

  err = myRTC.ReadTime(ti_back);
  cout << "new time is " << myRTC.ToString(ti_back) << endl;

}
