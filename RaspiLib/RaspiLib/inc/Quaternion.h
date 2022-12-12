#pragma once

namespace PiLib
{
  // quaternion class for 3d transformations
  // helper for gyro calculations
  class Quaternion
  {
    public:
      Quaternion(float w, float x, float y, float z);
      ~Quaternion();

      Quaternion(const Quaternion &qt);
      const Quaternion &operator =(const Quaternion &);

      const Quaternion operator *(const Quaternion &qt) const;

      const Quaternion normalized() const;
      const Quaternion conjugated() const;

      float Magnitude() const;

      float W() const {return m_w;}
      float X() const {return m_x;}
      float Y() const {return m_y;}
      float Z() const {return m_z;}

    private:
      float m_w=-1., m_x=0., m_y=0., m_z=0.;

      void normalize();
  };
}