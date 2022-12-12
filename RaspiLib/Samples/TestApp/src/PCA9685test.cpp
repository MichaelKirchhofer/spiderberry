#include "PCA9685test.h"

#include <RaspiLib.h>
#include <PCA9685.h>

#include <iostream>

using namespace PiLib;
using namespace std;

void doPCA9685SimpleTest()
{
  PCA9685 myDevice(0x40);

  cout << "initialized PCA9685 device 0x41" << endl;

  cout << "setting PWM frequency to 1000" << endl;
  // check frequency
  myDevice.SetPWMFrequency(1000);

  cout << "setting duty cycle to 0 on, 2048 off" << endl;
  myDevice.SetPWM(8, 0, 2048);

  cout << "waiting for 10 secs" << endl;  

  for (int i=0; i<10; i++)
  {
    Utils::delay_ms(1000);
    cout << ".";
  }

  cout << "  - done" << endl;

}

void moveLeg()
{
  PCA9685 myDevice(0x41);
  PCA9685 myDevice2(0x40);

  cout << "initialized PCA9685 device 0x41" << endl;

  cout << "setting PWM frequency to 50 Hz for Servos" << endl;
  // check frequency
  myDevice.SetPWMFrequency(50);
  myDevice2.SetPWMFrequency(50);

  // use leg1:
  // Servo1: #15, 90°
  // Servo2: #14, 90°
  // Servo3: #13, 0°

  cout << "stretch leg 1 ... ";
  myDevice.SetServoAngle(15, 90.);
  myDevice.SetServoAngle(14, 90.);
  myDevice.SetServoAngle(13, 0.);

  Utils::delay_ms(500);
  cout << " done" << endl;

  cout << "stretch leg 4 ... ";
  myDevice2.SetServoAngle(0, 90.);
  myDevice2.SetServoAngle(1, 90.);
  myDevice2.SetServoAngle(2, 180.);

  Utils::delay_ms(500);
  cout << " done" << endl;

  cout << "move legs forward ... ";
  myDevice.SetServoAngle(15,70.);
  myDevice2.SetServoAngle(0, 110.);

  Utils::delay_ms(2000);
  cout << " done" << endl;

  cout << "move legs back ... ";
  myDevice.SetServoAngle(15,110.);
  myDevice2.SetServoAngle(0, 70.);

  Utils::delay_ms(2000);
  cout << " done" << endl;

  cout << " .. and back to idle .. ";

  myDevice.SetServoAngle(15,90.);
  myDevice2.SetServoAngle(0, 90.);

  Utils::delay_ms(500);
  cout << " done" << endl;

  cout << "move middle legs up ... ";
  myDevice.SetServoAngle(14,110.);
  myDevice2.SetServoAngle(1, 70.);

  Utils::delay_ms(2000);
  cout << " done" << endl;

  cout << "move middle legs down ... ";
  myDevice.SetServoAngle(14,70.);
  myDevice2.SetServoAngle(1, 110.);

  Utils::delay_ms(2000);
  cout << " done" << endl;

  cout << " .. and back to idle .. ";

  myDevice.SetServoAngle(14,90.);
  myDevice2.SetServoAngle(1, 90.);

  Utils::delay_ms(500);
  cout << " done" << endl;

  cout << "move outer legs down ... ";
  myDevice.SetServoAngle(13,110.);
  myDevice2.SetServoAngle(2, 180.-110.);

  Utils::delay_ms(2000);
  cout << " done" << endl;

  cout << " .. and back to idle .. ";

  myDevice.SetServoAngle(13,0.);
  myDevice2.SetServoAngle(2, 180.);

  Utils::delay_ms(500);
  cout << " done" << endl;


}