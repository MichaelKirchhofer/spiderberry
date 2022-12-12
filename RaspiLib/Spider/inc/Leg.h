#pragma once

#include "SpiderServos.h"

#include <Vector3D.h>

#include <iostream>

namespace SpiderLib
{

  const int Leg1Len = 33;
  const int Leg2Len = 90;
  const int Leg3Len = 110;

  class Leg
  {
    public:
      // define a spider leg
      // we need 3 channels per leg, we need to know whether we are left or right
      // and the callback interface
      // servos are still controlled by Spider class
      Leg(std::uint8_t inner, std::uint8_t middle, std::uint8_t outer, bool left, SpiderServos *callback);
      ~Leg();

      // angle inner is neutral at 90
      // angle middle is neutral at 90
      // angle outer is neutral at 0 (right) or 180 (left)
      PiLib::ErrorCode setAngle(float inner, float middle, float outer);
      PiLib::ErrorCode setAngle(const PiLib::Vector3D &target) { return setAngle(target.X(), target.Y(), target.Z()); }

      PiLib::ErrorCode stretch();
      PiLib::ErrorCode idlePosition();

      // move the leg to the given leg position
      // leg x goes into neutral radial direction
      // leg y goes normal in the horizontal plane
      // leg z goes down in vertical direction
      // neutral leg position is (140, 0, 0)
      // leg on floor neutral is (140, 0, 25)
      PiLib::ErrorCode gotoLegPosition(const PiLib::Vector3D &pos);

      const PiLib::Vector3D &getLegPosition() {return m_legPosition;}
      const PiLib::Vector3D &getAngles() {return m_angles;}

      // transformation functions
      PiLib::Vector3D legPositionToAngles(const PiLib::Vector3D &position);
      PiLib::Vector3D anglesToLegPosition(const PiLib::Vector3D &angles);

      // make sure we do not cross borders ...
      float limit(float value, float vmin, float vmax)
      {
        if (value < vmin) value = vmin;
        if (value > vmax) value = vmax;
        return value;
      }

    private:
      // Servo Channels
      std::uint8_t m_inner = 0;
      std::uint8_t m_middle = 0;
      std::uint8_t m_outer = 0;
      bool m_left = false;

      // Servo callback
      SpiderServos *m_callback = nullptr;

      // local position & angles
      PiLib::Vector3D m_angles;
      PiLib::Vector3D m_legPosition;
  };


}