#include "adcSample.h"

#include <RaspiLib.h>
#include <PCF8591.h>
#include <ADS7830.h>

#include <iostream>

using namespace PiLib;
using namespace std;

void doI2CScan()
{
  II2c &iface = RaspiLib::GetInstance().GetI2c();

  ByteBuffer bb;
  ErrorCode err = iface.Find_Bus_Partners(bb);

  if (err != E_OK)
  {
    cerr << "cannot scan I2C bus, error " << err << endl;
    return;
  }

  cout << "Found " << bb.size() << " I2C slaves" << endl;
  for (int i=0; i<bb.size(); i++)
    cout << (int)bb[i] << ", ";

  cout << endl;
}

// PCF8591 is at I2C address 0x48
void checkDAC()
{
  PCF8591 myDAC(0x48);

  int received = 0;

  for (int i=0; i<256; i++)
  {
    myDAC.Write(i);
    myDAC.Read(1, received);

    cout << "Written " << i << ", read back " << received << endl;
  }

}

float ReadADC(ADS7830 &theADC, int channel)
{
  float value = 0.;
  int received = 0;

  cout << "measuring channel " << channel << ": ";

  // get 10 values
  for (int j=0; j<10; j++)
  {
    theADC.Read(channel, received);
    cout << " " << received;
    value += received;
  }

  value *= 5.;   // 5V
  value /= 255.; // full resolution
  value /= 10.;  // one value

  cout << " -> " << value << " volts" << endl;

  return value;
}

void checkBattery()
{
  ADS7830 myADC(0x48);

  for (int i=0; i<10; ++i)
  {
    ReadADC(myADC, 0);
    ReadADC(myADC, 1);
    ReadADC(myADC, 2);
    ReadADC(myADC, 3);
    ReadADC(myADC, 4);
    ReadADC(myADC, 5);
    ReadADC(myADC, 6);
    ReadADC(myADC, 7);

    Utils::delay_ms(1000);
  }
}