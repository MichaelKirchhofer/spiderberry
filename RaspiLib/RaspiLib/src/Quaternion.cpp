#include "Quaternion.h"

#include <math.h>

using namespace PiLib;

Quaternion::Quaternion(float w, float x, float y, float z) : m_w(w), m_x(x), m_y(y), m_z(z)
{
}

Quaternion::~Quaternion()
{}

Quaternion::Quaternion(const Quaternion &qt)
{
    *this = qt;
}
const Quaternion & Quaternion::operator =(const Quaternion &qt)
{
  if (this != &qt)
  {
    m_w = qt.m_w;
    m_x = qt.m_x;
    m_y = qt.m_y;
    m_z = qt.m_z;
  }

  return *this;
}

const Quaternion Quaternion::operator *(const Quaternion &qt) const
{
// Quaternion multiplication is defined by:
//     (Q1 * Q2).w = (w1w2 - x1x2 - y1y2 - z1z2)
//     (Q1 * Q2).x = (w1x2 + x1w2 + y1z2 - z1y2)
//     (Q1 * Q2).y = (w1y2 - x1z2 + y1w2 + z1x2)
//     (Q1 * Q2).z = (w1z2 + x1y2 - y1x2 + z1w2

  float nw = W()*qt.W() - X()*qt.X() - Y()*qt.Y() - Z()*qt.Z();
  float nx = W()*qt.X() + X()*qt.W() + Y()*qt.Z() - Z()*qt.Y();
  float ny = W()*qt.Y() - X()*qt.Z() + Y()*qt.W() + Z()*qt.X();
  float nz = W()*qt.Z() + X()*qt.Y() - Y()*qt.X() + Z()*qt.W();

  return Quaternion(nw, nx, ny, nz);
}

const Quaternion Quaternion::normalized() const
{
  Quaternion ret(*this);
  ret.normalize();
  return ret;
}

const Quaternion Quaternion::conjugated() const
{
  Quaternion ret(W(), -X(), -Y(), -Z());
  return ret;
}

float Quaternion::Magnitude() const
{
  return (float) sqrt(W()*W() + X()*X() + Y()*Y() + Z()*Z());
}

void Quaternion::normalize()
{
  float mag = Magnitude();

  m_w /= mag;
  m_x /= mag;
  m_y /= mag;
  m_z /= mag;
}
