#include "Leg.h"

#include "SpiderServos.h"

#include <cmath>
const float PI = 3.14159265f; 

using namespace SpiderLib;
using namespace PiLib;
using namespace std;

Leg::Leg(uint8_t inner, uint8_t middle, uint8_t outer, bool left, SpiderServos *callback) : 
  m_inner(inner), m_middle(middle), m_outer(outer), m_left(left), m_callback(callback)
{ }

Leg::~Leg()
{ }

ErrorCode Leg::setAngle(float inner, float middle, float outer)
{
  // inner servos and middle servos have offset of 90
  // inner servo is already recognized in transformation, delivers 90 for idle
  // middle must be corrected

  if (inner < 0.  || inner > 180.)  return E_INVALID_PARAM;
  if (middle < -90. || middle > 90.) return E_INVALID_PARAM;
  if (outer < 0.  || outer > 180.)  return E_INVALID_PARAM;

  if (m_callback)
  {
    m_callback->SetServoAngle(m_inner, inner);
    m_callback->SetServoAngle(m_middle, (float) ((m_left) ? 90. + middle : 90 - middle));
    // take care of left/right here
    m_callback->SetServoAngle(m_outer, (float)((m_left) ? 180. - outer : outer));

    m_angles = Vector3D(inner, middle, outer);

    return E_OK;
  }

  return E_NOT_INIT;
}

ErrorCode Leg::stretch()
{
  ErrorCode err = setAngle(0.,0.,0.);
  if (err == E_OK)
    m_legPosition = anglesToLegPosition(getAngles());

  return err;
}

ErrorCode Leg::idlePosition()
{
  Vector3D idle(140.,0.,0.);
  return gotoLegPosition(idle);
}

ErrorCode Leg::gotoLegPosition(const Vector3D &pos)
{
  Vector3D angles = legPositionToAngles(pos);

  setAngle(angles);
  // make sure we have the "real" position
  m_legPosition = anglesToLegPosition(angles);

  return E_OK;
}

// transformation functions
Vector3D Leg::legPositionToAngles(const Vector3D &position)
{
  float inner = 0., middle = 0., outer = 0.;

  inner = PI/2.f - atan2(position.Y(), position.X());

  float proj_x = Leg1Len * sin(inner);
  float proj_y = Leg1Len * cos(inner);
  float proj_z = 0.;

  float len23 = sqrt((position.X() - proj_x) * (position.X() - proj_x) + 
                     (position.Y() - proj_y) * (position.Y() - proj_y) + 
                     (position.Z() - proj_z) * (position.Z() - proj_z));

  float w = limit( (position.Z() - proj_z) / len23, -1., 1. );
  float v = limit( (Leg2Len*Leg2Len + len23*len23 - Leg3Len*Leg3Len)/(2*Leg2Len*len23), -1., 1.);
  float u = limit( (Leg2Len*Leg2Len + Leg3Len*Leg3Len-len23*len23)/(2*Leg2Len*Leg3Len), -1., 1.);

  middle = asin(w) - acos(v);
  outer = PI - acos(u);

  // now convert to degrees
  inner = inner * 180.f / PI;
  middle = middle * 180.f / PI;
  outer = outer * 180.f / PI;

  return Vector3D(inner, middle, outer);
}

Vector3D Leg::anglesToLegPosition(const Vector3D &angles)
{
  float inner = angles.X() * PI / 180.f;
  float middle = angles.Y() * PI / 180.f;
  float outer = angles.Z() * PI / 180.f;

  float x = Leg3Len * sin(inner) * cos(middle + outer) + Leg2Len * sin(inner) * cos(middle) + Leg1Len * sin(inner);
  float y = Leg3Len * cos(inner) * cos(middle + outer) + Leg2Len * cos(inner) * cos(middle) + Leg1Len * cos(inner);
  float z = Leg3Len * sin(middle + outer) + Leg2Len * sin(middle);

  return Vector3D(x, y, z);
}
