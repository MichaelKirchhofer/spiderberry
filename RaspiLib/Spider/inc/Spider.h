#pragma once

#include <ErrorCode.h>

#include "SpiderServos.h"

#include <Vector3D.h>
#include "world.h"

namespace PiLib
{
  class PCA9685;
}

namespace SpiderLib
{
  class Leg;

  typedef std::pair<float, float> XYType;
 
  // this is the spider main class
  // container fo all the peripherals
  class Spider : public SpiderServos
  {
    // lift the foot when moving
    const float c_liftFoot = 15.0;
    // the parts for one step cycle
    const int c_steps = 64;

    public:
      Spider();
      ~Spider();

      PiLib::ErrorCode Init();
      PiLib::ErrorCode Reset();

      // init leg positions - can be done without Init() -> for unit tests
      PiLib::ErrorCode InitLegs();

      // move the spider
      // start walking with default speed
      // speed -10 .. 0 .. 10
      // negative speed means walk back
      // speed 0 means stop
      PiLib::ErrorCode Walk(int speed = 10);
      PiLib::ErrorCode Stop() { return Walk(0);}

      // goto given height (0 is contact, 25 is default)
      PiLib::ErrorCode Raise(float value_mm = 25.);

      // turn spider in clockwise direction, negative angle -> counterclockwise
      PiLib::ErrorCode Turn(int angle);

      // move legs
      PiLib::ErrorCode StretchLegs();
      PiLib::ErrorCode LegsToIdle();

      // inner functions
      virtual PiLib::ErrorCode SetServoAngle(std::uint8_t channel, float angle) override;

      // move the spider, speed is for total cycle, minimum 64 ms
      // later: return false if stopped
      bool run(float dx, float dy, int speed_ms = 1000, float angle = 0., int gait = 1);

      const PointArray &LegCoordinates() const {return m_legCoordinates;}

      private:

      // make sure we do not cross borders ...
      float limit(float value, float vmin, float vmax)
      {
        if (value < vmin) value = vmin;
        if (value > vmax) value = vmax;
        return value;
      }

      Leg *m_leftFront = nullptr;
      Leg *m_leftMiddle = nullptr;
      Leg *m_leftBack = nullptr;
      Leg *m_rightFront = nullptr;
      Leg *m_rightMiddle = nullptr;
      Leg *m_rightBack = nullptr;

      PiLib::PCA9685 *m_Servo0 = nullptr;  // 0x41
      PiLib::PCA9685 *m_Servo1 = nullptr;  // 0x40

      // the spider main loop, running as separate task
      void work();

      // in case of init failure make sure all pointers are released
      void cleanup();

      // -> enum LegPosition
      // leg positions in spider system
      PointArray m_legCoordinates;

      // move legs to m_legCoordinates
      void moveLegs(const PointArray &points);

      void calcStep(PiLib::Vector3D &pos, const XYType &delta, float Z, LegPosition leg, int part);
      void calcStep2(PiLib::Vector3D &pos, const XYType &delta, LegPosition leg, int part, bool other);

      // return true if there is an obstacle
      bool checkObstacle();

      // for testing, switch off during production!
      void dumpCoordinates(int line, const PointArray &points);

  };
}