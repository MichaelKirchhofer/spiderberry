#include "DummyCallback.h"

#include <iostream>

using namespace PiLib;
using namespace SpiderLib;
using namespace std;

ErrorCode DummyCallback::SetServoAngle(std::uint8_t channel, float angle)
{
  cout << "SpiderServos: " << m_text << " setting channel " << to_string(channel) << " to angle " << to_string(angle) << endl;

  if (m_expectations.find(channel) == m_expectations.end())
  {
    cout << " !!!!! call not expected !!!!!" << endl;
    return E_INVALID_PARAM;
  }

  if (! compare_float(m_expectations[channel],angle))
  {
    cout << " ***** angle not expected! ***** (" << to_string(m_expectations[channel]) << ")" << endl; 
    return E_UNEXPECTED_VALUE;
  }

  return E_OK;
}

void DummyCallback::addExpectation(uint8_t channel, float angle) 
{
  m_expectations[channel] = angle;
}

bool compare_float(float x, float y, float epsilon)
{
  cout << "float compare " << x << " with " << y << "  epsilon " << epsilon << endl;
   if(fabs(x - y) < epsilon)
      return true; //they are same
      
  return false; //they are not same
}