#pragma once

#include <Vector3D.h>
#include <vector>

typedef std::vector<PiLib::Vector3D> PointArray;

// this file defines the coordinate system for the spider
// it contains all relevant coordinate information

namespace SpiderLib
{
  enum LegPosition
  {
    RightFront = 0,
    RightMiddle = 1,
    RightBack = 2,
    LeftFront = 3,
    LeftMiddle = 4,
    LeftBack = 5,
  };

  // idle positions for each leg
  const PiLib::Vector3D LeftFrontPosition(-137.1, 189.4, 0.);
  const PiLib::Vector3D LeftMiddlePosition(-225., 0., 0.);
  const PiLib::Vector3D LeftBackPosition(-137.1, -189.4, 0.);
  const PiLib::Vector3D RightFrontPosition(137.1, 189.4, 0.);
  const PiLib::Vector3D RightMiddlePosition(225., 0., 0.);
  const PiLib::Vector3D RightBackPosition(137.1, -189.4, 0.);

  class WorldTransformation
  {
    public:
      // for now just offer simple coordinate transforms
      static bool getLegCoordinates(const PointArray & wc, PointArray &lc);

      static PiLib::Vector3D WorldToLeg(const PiLib::Vector3D &wc, LegPosition which);
      static PiLib::Vector3D LegToWorld(const PiLib::Vector3D &lc, LegPosition which);

      // later add IMU corrections as well
  };

};