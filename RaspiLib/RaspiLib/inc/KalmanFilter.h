#pragma once

namespace PiLib
{
  // one dimensional Kalman Filter
  // time dependency is not considered
  // state transition matrix A = 1
  // state_to_measurement matrix H = 1

  class KalmanFilter
  {
    public:
      KalmanFilter(float q, float r);
      ~KalmanFilter();

      float doEstimation(float value);

    private:
      float m_q;  // process noise covariance
      float m_r;  // measurement covariance

      float m_a = 1.;  // state transition
      float m_h = 1.;  // state_to_measurement

      float m_x_old;   // value x of last cycle
      float m_p_old;   // value p of last cycle
  };

}