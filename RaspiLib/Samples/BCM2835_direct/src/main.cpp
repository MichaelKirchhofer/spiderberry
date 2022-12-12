#include <bcm2835.h>
#include <iostream>

#include "adctest.h"

using namespace std;

int main()
{
  if (!bcm2835_init())
  {
    cerr << "bcm2835_init failed. Are you running as root??" << endl;
    return 1;
  }
      
  // I2C begin     
  if (!bcm2835_i2c_begin())
  {
    cerr << "bcm2835_i2c_begin failed. Are you running as root??" << endl;
	  return 1;
  }

  // run at 100 kHz
  bcm2835_i2c_setClockDivider(BCM2835_I2C_CLOCK_DIVIDER_2500);

  // do a bus scan
  char buf = 0;
  int code = 0;
  char reg = 0;

  std::cout << endl;
  for(int i = 0; i<0x10; i++) 
  {
    std::cout << "\t+" << std::hex << i;
  }    
  
  std::cout << endl;
  for(int i = 0; i<7; i++) 
  {
    std::cout << std::hex << i*0x10;
    for(int j=0; j<0x10; j++) 
    {
      bcm2835_i2c_setSlaveAddress(i * 0x10 + j);        
      code = bcm2835_i2c_read_register_rs(&reg, &buf, 1);
      std::cout << "\t" << (code==0 ? "O" : ".");
    }
    std::cout << endl;
  }

  ADCTest test(0x48);

  for (int i=0; i<256; i+=10)
  {
    cout << "ADC: writing value " << i << endl;
    test.Write(i);

    int val = 0;
    test.Read(1, val);
    cout << "ADC: read value " << val << endl;

    bcm2835_delay(100);
  }

  // I2C end
  bcm2835_i2c_end();

  bcm2835_close();
  cout << "... done!" << endl;
  return 0;
}