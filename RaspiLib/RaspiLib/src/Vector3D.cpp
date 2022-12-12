#include "Vector3D.h"

#include <cmath>

using namespace PiLib;

Vector3D::Vector3D(float x, float y, float z) : m_x(x), m_y(y), m_z(z)
{ }

Vector3D::~Vector3D()
{ }

Vector3D::Vector3D(const Vector3D &v)
{
  *this = v;
}

const Vector3D & Vector3D::operator =(const Vector3D &v)
{
  if (this != &v)
  {
    m_x = v.m_x;
    m_y = v.m_y;
    m_z = v.m_z;
  }

  return *this;
}

const Vector3D Vector3D::operator +(const Vector3D &v) const
{
  Vector3D ret(X() + v.X(), Y() + v.Y(), Z() + v.Z());
  return ret;
}

const Vector3D Vector3D::operator -(const Vector3D &v) const
{
  Vector3D ret(X() - v.X(), Y() - v.Y(), Z() - v.Z());
  return ret;
}

bool Vector3D::operator==(const Vector3D &v) const
{
  return isEqual(v, 0.01);
}

// compare with tolerance
bool Vector3D::isEqual(const Vector3D &v, float epsilon) const
{
  if(fabs(X() - v.X()) > epsilon) return false;
  if(fabs(Y() - v.Y()) > epsilon) return false;
  if(fabs(Z() - v.Z()) > epsilon) return false;

  return true;
}


// multiply with constant      
const Vector3D Vector3D::operator *(float x) const
{
  Vector3D ret(X()*x, Y()*x, Z()*x);
  return ret;
}

// multiply with vector - in product
float Vector3D::operator *(const Vector3D &v) const
{
  float res = X()*v.X() + Y()*v.Y() + Z()*v.Z();
  return res;
}

// multiply with vector - x product
const Vector3D Vector3D::xProduct(const Vector3D &v) const
{
  float nx = Y()*v.Z() - Z()*v.Y();
  float ny = Z()*v.X() - X()*v.Z();
  float nz = X()*v.Y() - Y()*v.X();

  Vector3D res(nx, ny, nz);
  return res;
}

const Vector3D Vector3D::normalized() const
{
  float mag = Magnitude();
  Vector3D res(X() / mag, Y() / mag, Z()/mag);
  return res;
}

float Vector3D::Magnitude() const
{
  return (float) sqrt(X()*X() + Y()*Y() + Z()*Z());
}

const Vector3D Vector3D::Rotate(const Quaternion &qt)
{
  // P_out = qt * P_in * conj(qt)
  // - P_out is the output vector
  // - qt is the orientation quaternion
  // - P_in is the input vector (a*aReal)
  // - conj(qt) is the conjugate of the orientation quaternion (qt=[w,x,y,z], qt*=[w,-x,-y,-z])

  Quaternion p(0, m_x, m_y, m_z);

  p = qt * p;
  p = p * qt.conjugated();

  Vector3D res(p.X(), p.Y(), p.Z());
  return res;
}

std::string Vector3D::ToString() const
{
  std::string ret = "X: " + std::to_string(X()) + " Y:" + std::to_string(Y()) + " Z:" + std::to_string(Z());
  return ret;
}
