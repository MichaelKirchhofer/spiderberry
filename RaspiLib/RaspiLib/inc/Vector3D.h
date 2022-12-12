#pragma once

#include "Quaternion.h"

#include <string>

namespace PiLib
{
  // Vector3D class for 3d transformations
  // helper for gyro calculations
  class Vector3D
  {
    public:
      Vector3D(float x=0., float y=0., float z=0.);
      ~Vector3D();

      Vector3D(const Vector3D &);
      const Vector3D & operator =(const Vector3D &);

      const Vector3D operator +(const Vector3D &) const;
      const Vector3D operator -(const Vector3D &) const;

      bool operator==(const Vector3D &) const;
      // compare with tolerance
      bool isEqual(const Vector3D &, float epsilon) const;

      // multiply with constant      
      const Vector3D operator *(float x) const;
      // multiply with vector - in product
      float operator *(const Vector3D &) const;
      // multiply with vector - x product
      const Vector3D xProduct(const Vector3D &) const;

      const Vector3D normalized() const;

      float Magnitude() const;

      float X() const {return m_x;}
      float Y() const {return m_y;}
      float Z() const {return m_z;}

      const Vector3D Rotate(const Quaternion &qt);

      // short version to get current values as string
      std::string ToString() const;

    private:
      float m_x=0., m_y=0., m_z=0.;
  };
}
