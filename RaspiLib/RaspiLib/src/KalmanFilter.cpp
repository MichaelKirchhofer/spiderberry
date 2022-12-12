#include "KalmanFilter.h"

#include <math.h>

using namespace PiLib;

KalmanFilter::KalmanFilter(float q, float r) : m_q(q), m_r(r)
{ }

KalmanFilter::~KalmanFilter()
{ }

float KalmanFilter::doEstimation(float value)
{
  // predict state and state error at measurement time
  float x_p = m_x_old;
  float p_p = m_p_old + m_q;

  // compute Kalman gain
  float k = p_p / (p_p + m_r);

  // estimate state and state error at measurement time
  float x = x_p + k * (value - x_p);
  float p = p_p - k * p_p;

  m_x_old = x;
  m_p_old = p;

  // return kalman value
  return x;
}
