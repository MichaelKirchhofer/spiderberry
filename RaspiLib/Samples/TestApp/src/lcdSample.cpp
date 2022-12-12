
#include <RaspiLib.h>
#include <lcd.h>
#include <LCD_I2C.h>

using namespace PiLib;

const RaspiPinLabel RS = PIN15_GPIO22;
const RaspiPinLabel STRB = PIN23_GPIO11_SCKL;
const RaspiPinLabel D0 = PIN16_GPIO23;
const RaspiPinLabel D1 = PIN19_GPIO10_MOSI;
const RaspiPinLabel D2 = PIN21_GPIO9_MISO;
const RaspiPinLabel D3 = PIN22_GPIO25;

void doLCD()
{
  LCD myLCD;

  ErrorCode err = myLCD.init(true, RS, STRB, D0, D1, D2, D3, D0, D1, D2, D3);
  if (err == E_OK)
  {
    myLCD.position(5,0);
    myLCD.putString("Hello");
    myLCD.position(5,1);
    myLCD.putString("World!");
  }

  
}

void doLCD_I2C()
{
  LCD_I2C myLCD;   // use default address

  myLCD.Init();

  myLCD.Write(5,0,"Hello");
  myLCD.Write(5,1,"world!");
}
