#include "RaspiLib.h"

#include <iostream>

#include "blinker.h"
#include "lcdSample.h"

#include "adcSample.h"
#include "RTCTest.h"

#include "USound.h"
#include "GyroTest.h"

#include "PCA9685test.h"

#include "sensehat.h"

using namespace PiLib;
using namespace std;

void Blinker();

int main(int argc, char *argv[])
{
  unsigned mVersion, lVersion;
  RaspiLib::LibVersion(mVersion, lVersion);

  cout << "using PiLib Version " << mVersion << "." << lVersion << endl;

  bool isOk = RaspiLib::Startup();
  if (! isOk)
  {
    cerr << "Problems on initializing PiLib - aborting" << endl;
    return -1;
  }

  IBoard &board = RaspiLib::GetInstance().GetBoard();

  const BoardInfo &info = board.GetBoardInfo();
  cout << info.ToString() << endl;

  // check pins
  // Blinker();

  // output something
  // doLCD();
  // doLCD_I2C();

  // check I2C bus
  doI2CScan();

  // checkDAC();

  testRTC();

  // USoundDistance();

  // doGyroTest();
  
  // doPCA9685SimpleTest();

  // checkBattery();

  // moveLeg();

  doSenseHatTest();

  return 0;
}
