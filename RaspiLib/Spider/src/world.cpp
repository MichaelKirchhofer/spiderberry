#include "world.h"

#include <cmath>
const float PI = 3.14159265f; 

// spider geometry
const float anglesForLegs[]  = { 54.f,   0.f, -54.f, 126.f, 180.f, -126.f};
const float xoffsetForLegs[] = {-94.f, -85.f, -94.f, -94.f, -85.f,  -94.f};
const float zoffsetForLegs[] = {-14.f, -14.f, -14.f, -14.f, -14.f,  -14.f};

using namespace SpiderLib;
using namespace PiLib;
using namespace std;

bool WorldTransformation::getLegCoordinates(const PointArray & wc, PointArray &lc)
{
  lc[LeftBack] = WorldToLeg(wc[LeftBack], LeftBack);
  lc[LeftMiddle] = WorldToLeg(wc[LeftMiddle], LeftMiddle);
  lc[LeftFront] = WorldToLeg(wc[LeftFront], LeftFront);
  lc[RightBack] = WorldToLeg(wc[RightBack], RightBack);
  lc[RightMiddle] = WorldToLeg(wc[RightMiddle], RightMiddle);
  lc[RightFront] = WorldToLeg(wc[RightFront], RightFront);

  return true;
}

Vector3D WorldTransformation::WorldToLeg(const Vector3D &wc, LegPosition which)
{
  if (which > LeftBack) return Vector3D(0,0,0);

  float x, y, z;

  x = wc.X() * cos(anglesForLegs[which] * PI / 180.) + wc.Y() * sin(anglesForLegs[which] * PI / 180.) + xoffsetForLegs[which];
  y = -wc.X() * sin(anglesForLegs[which] * PI / 180.) + wc.Y() * cos(anglesForLegs[which] * PI / 180.);
  z = wc.Z() + zoffsetForLegs[which];

  return Vector3D(x, y, z);
}

Vector3D WorldTransformation::LegToWorld(const Vector3D &lc, LegPosition which)
{
  return Vector3D(0,0,0);
}
