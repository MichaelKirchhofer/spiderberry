#include <I2C.h>

#include <iostream>

using namespace PiLib;
using namespace std;

int main()
{
  I2C myI2C;

  cout << "starting i2c in smbus mode" << endl;

  ErrorCode err = myI2C.init();
  if (err != E_OK)
  {
    cerr << "cannot initialize I2C smbus interface" << endl;
    exit(1);
  }

  ByteBuffer slaves;

  err = myI2C.Find_Bus_Partners(slaves);
  if (err != E_OK)
  {
    cerr << "cannot determine slave partners on bus" << endl;
  }
  else
  {
    for (auto item : slaves)
    {
      cout << "found slave at address " << hex << "0x" << (int)item << dec << endl;
    }
  }

  cout << "shutting down i2c" << endl;

  myI2C.shutdown();
}
